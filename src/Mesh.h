#pragma once
#include "raymath.h"
#include <glad/glad.h>
#include <vector>

enum class ParShapesType
{
    MESH_TETRAHEDRON,
    MESH_CUBE,
    MESH_OCTAHEDRON,
    MESH_DODECAHEDRON,
    MESH_ICOSAHEDRON,
    MESH_TYPE_COUNT
};

struct Mesh
{
    //CPU related
    std::vector<Vector3> positions;
    std::vector<Vector2> tcoords; //texture coordinates
    std::vector<Vector3> normals;
    std::vector<GLuint> indices;

    //GPU related
    GLuint vbo = GL_NONE; //vertex buffer
    GLuint tbo = GL_NONE; //tcoords buffer
    GLuint nbo = GL_NONE; //normals buffer
    GLuint ebo = GL_NONE; //index buffer (= ibo)
    GLuint vao = GL_NONE;
};

void CreateMesh(Mesh *mesh,
                int vertexCount,
                int indexCount,
                const Vector3 *positions,
                const Vector2 *tcoords,
                const Vector3 *normals,
                const GLuint *indices);
void LoadMesh(Mesh *mesh);
void LoadMesh(Mesh *mesh, ParShapesType type);
void LoadMesh(Mesh *mesh, const char *path);
void UnloadMesh(Mesh *mesh);
void DrawMesh(const Mesh &mesh, GLuint shaderProgram, GLint u_mvp = -1, Matrix mat = MatrixIdentity());