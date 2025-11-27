#define _CRT_SECURE_NO_WARNINGS //to disable warnings from file_open() of fast_obj.h
#include "Mesh.h"
#include <cassert>
#include <cstring>
#include <iostream>

#define RAYMATH_IMPLEMENTATION
#include "raymath.h"
#define PAR_SHAPES_IMPLEMENTATION
#include <par_shapes/par_shapes.h>
#define FAST_OBJ_IMPLEMENTATION
#include <fast_obj/fast_obj.h>

//----------------------------------------------------------------------------------
//helper functions not exposed in headers
static par_shapes_mesh *GeneratePlatonic(MeshType type);
static par_shapes_mesh *GenerateParametric(MeshType type,
                                           int slice,
                                           int stack);
static par_shapes_mesh *GenerateParametricRadius(MeshType type,
                                                 int slice,
                                                 int stack,
                                                 float radius);
static void GenerateParShapesMesh(Mesh *mesh, par_shapes_mesh *par);
//----------------------------------------------------------------------------------

void CreateMesh(Mesh *mesh,
                int vertexCount,
                int indexCount,
                const Vector3 *positions,
                const Vector2 *tcoords,
                const Vector3 *normals,
                const GLuint *indices)
{
    for (int i = 0; i < vertexCount; i++)
    {
        mesh->positions.push_back(positions[i]);
        mesh->tcoords.push_back(tcoords[i]);
        mesh->normals.push_back(normals[i]);
    }
    for (int i = 0; i < indexCount; i++)
    {
        mesh->indices.push_back(indices[i]);
    }
}
void LoadMeshGPU(Mesh *mesh)
{
    assert(!mesh->positions.empty());

    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);

    glGenBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh->positions.size() * sizeof(Vector3), mesh->positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    if (!mesh->tcoords.empty())
    {
        glGenBuffers(1, &mesh->tbo);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->tbo);
        glBufferData(GL_ARRAY_BUFFER, mesh->tcoords.size() * sizeof(Vector2), mesh->tcoords.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (GLvoid *)0);
        glEnableVertexAttribArray(1);
    }

    if (!mesh->normals.empty())
    {
        glGenBuffers(1, &mesh->nbo);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->nbo);
        glBufferData(GL_ARRAY_BUFFER, mesh->normals.size() * sizeof(Vector3), mesh->normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (GLvoid *)0);
        glEnableVertexAttribArray(2);
    }

    if (!mesh->indices.empty())
    {
        glGenBuffers(1, &mesh->ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(GLuint), mesh->indices.data(), GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void LoadMesh(Mesh *mesh, MeshType type)
{
    par_shapes_mesh *par = GeneratePlatonic(type);
    GenerateParShapesMesh(mesh, par);
    par_shapes_free_mesh(par);
    LoadMeshGPU(mesh);
}
void LoadMesh(Mesh *mesh, MeshType type, int slice, int stack)
{
    par_shapes_mesh *par = GenerateParametric(type, slice, stack);
    GenerateParShapesMesh(mesh, par);
    par_shapes_free_mesh(par);
    LoadMeshGPU(mesh);
}
void LoadMesh(Mesh *mesh, MeshType type, int slice, int stack, float radius)
{
    par_shapes_mesh *par = GenerateParametricRadius(type, slice, stack, radius);
    GenerateParShapesMesh(mesh, par);
    par_shapes_free_mesh(par);
    LoadMeshGPU(mesh);
}
//NOTE: non-deduplicated vertices!
void LoadMesh(Mesh *mesh, const char *path)
{
    fastObjMesh *obj = fast_obj_read(path);
    assert(obj != nullptr);

    for (size_t i = 0; i < obj->index_count; i++)
    {
        unsigned int p_idx = obj->indices[i].p; //positions
        unsigned int t_idx = obj->indices[i].t; //tcoords
        unsigned int n_idx = obj->indices[i].n; //normals

        Vector3 pos;
        pos.x = obj->positions[p_idx * 3 + 0];
        pos.y = obj->positions[p_idx * 3 + 1];
        pos.z = obj->positions[p_idx * 3 + 2];
        mesh->positions.push_back(pos);

        if (obj->texcoords != nullptr)
        {
            Vector2 tc;
            tc.x = obj->texcoords[t_idx * 2 + 0];
            tc.y = obj->texcoords[t_idx * 2 + 1];
            mesh->tcoords.push_back(tc);
        }

        Vector3 norm;
        norm.x = obj->normals[n_idx * 3 + 0];
        norm.y = obj->normals[n_idx * 3 + 1];
        norm.z = obj->normals[n_idx * 3 + 2];
        mesh->normals.push_back(norm);

        mesh->indices.push_back((GLuint)i);
    }

    fast_obj_destroy(obj);
    LoadMeshGPU(mesh);
}
void UnloadMesh(Mesh *mesh)
{
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->tbo);
    glDeleteBuffers(1, &mesh->nbo);
    glDeleteBuffers(1, &mesh->ebo);
    glDeleteVertexArrays(1, &mesh->vao);
    mesh->positions.resize(0);
    mesh->tcoords.resize(0);
    mesh->normals.resize(0);
    mesh->indices.resize(0);
}
void DrawMesh(const Mesh &mesh,
              GLuint shaderProgram,
              GLint u_mvp,
              Matrix mat,
              GLuint texture)
{
    glUseProgram(shaderProgram);
    if (u_mvp != -1) //if a uniform (of mat4) exists
        glUniformMatrix4fv(u_mvp, 1, GL_FALSE, MatrixToFloat(mat));
    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    if (texture != -1)
        glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, GLsizei(mesh.indices.size()), GL_UNSIGNED_INT, 0);
    //TODO: if no ebo then use glDrawArrays
}

//----------------------------------------------------------------------------------
//helper functions implementation

static par_shapes_mesh *GeneratePlatonic(MeshType type)
{
    par_shapes_mesh *par = nullptr;
    switch (type)
    {
        case MeshType::PAR_ICOSAHEDRON:
            par = par_shapes_create_icosahedron();
            break;
        case MeshType::PAR_DODECAHEDRON:
            par = par_shapes_create_dodecahedron();
            break;
        case MeshType::PAR_OCTAHEDRON:
            par = par_shapes_create_octahedron();
            break;
        case MeshType::PAR_TETRAHEDRON:
            par = par_shapes_create_tetrahedron();
            break;
        case MeshType::PAR_CUBE:
            par = par_shapes_create_cube();
            //par_shapes cube's centre is in a corner by default
            par_shapes_translate(par, -0.5f, -0.5f, -0.5f);
            break;
    }
    return par;
}
static par_shapes_mesh *GenerateParametric(MeshType type,
                                           int slice,
                                           int stack)
{
    par_shapes_mesh *par = nullptr;
    switch (type)
    {
        case MeshType::PAR_CYLINDER:
            par = par_shapes_create_cylinder(slice, stack);
            break;
        case MeshType::PAR_CONE:
            par = par_shapes_create_cone(slice, stack);
            break;
        case MeshType::PAR_DISK:
            par = par_shapes_create_parametric_disk(slice, stack);
            break;
        case MeshType::PAR_SPHERE:
            par = par_shapes_create_parametric_sphere(slice, stack);
            break;
        case MeshType::PAR_HEMISPHERE:
            par = par_shapes_create_hemisphere(slice, stack);
            break;
        case MeshType::PAR_PLANE:
            par = par_shapes_create_plane(slice, stack);
            break;
    }
    return par;
}
static par_shapes_mesh *GenerateParametricRadius(MeshType type,
                                                 int slice,
                                                 int stack,
                                                 float radius)
{
    par_shapes_mesh *par = nullptr;
    switch (type)
    {
        //Platonic Solids
        case MeshType::PAR_TORUS:
            par = par_shapes_create_torus(slice, stack, radius);
            break;
    }
    return par;
}
static void GenerateParShapesMesh(Mesh *mesh, par_shapes_mesh *par)
{
    assert(par != nullptr);
    par_shapes_compute_normals(par);

    mesh->positions.resize(par->npoints);
    Vector3 *par_positions = reinterpret_cast<Vector3 *>(par->points);
    memcpy(mesh->positions.data(), par_positions, par->npoints * sizeof(Vector3));

    mesh->normals.resize(par->npoints);
    Vector3 *par_normals = reinterpret_cast<Vector3 *>(par->normals);
    memcpy(mesh->normals.data(), par_normals, par->npoints * sizeof(Vector3));

    if (par->tcoords != nullptr) //platonic par_shapes doesn't have tcoords
    {
        mesh->tcoords.resize(par->npoints);
        Vector2 *par_tcoords = reinterpret_cast<Vector2 *>(par->tcoords);
        memcpy(mesh->tcoords.data(), par_tcoords, par->npoints * sizeof(Vector2));
    }

    mesh->indices.resize(3 * par->ntriangles);
    //cannot use memcpy here since par_shapes uses uint16_t, and I am using GLuint
    for (size_t i = 0; i < 3 * par->ntriangles; i++)
        mesh->indices[i] = par->triangles[i];
}