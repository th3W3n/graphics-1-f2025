#define _CRT_SECURE_NO_WARNINGS //to disable warnings from file_open() of fast_obj.h
#include "Mesh.h"

#define RAYMATH_IMPLEMENTATION
#include "raymath.h"
#define PAR_SHAPES_IMPLEMENTATION
#include <par_shapes/par_shapes.h>
#define FAST_OBJ_IMPLEMENTATION
#include <fast_obj/fast_obj.h>

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
void LoadMesh(Mesh *mesh)
{
    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);

    glGenBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh->positions.size() * sizeof(Vector3), mesh->positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &mesh->tbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->tbo);
    glBufferData(GL_ARRAY_BUFFER, mesh->tcoords.size() * sizeof(Vector2), mesh->tcoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (GLvoid *)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &mesh->nbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->nbo);
    glBufferData(GL_ARRAY_BUFFER, mesh->normals.size() * sizeof(Vector3), mesh->normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (GLvoid *)0);
    glEnableVertexAttribArray(2);

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
void LoadMesh(Mesh *mesh, const char *path)
{
    fastObjMesh *obj = fast_obj_read(path);
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
void DrawMesh(const Mesh &mesh, GLuint shaderProgram, GLint u_mvp)
{
    glUseProgram(shaderProgram);
    if (u_mvp != -1)
        glUniformMatrix4fv(u_mvp, 1, GL_FALSE, MatrixToFloat(MatrixIdentity()));
    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glDrawElements(GL_TRIANGLES, GLsizei(mesh.indices.size()), GL_UNSIGNED_INT, 0);
}