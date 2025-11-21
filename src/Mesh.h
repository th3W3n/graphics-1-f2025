#pragma once
#include "raymath.h"
#include <glad/glad.h>
#include <vector>

struct Mesh
{
    std::vector<Vector3> positions;
    std::vector<Vector3> tcoords; //texture coordinates
    std::vector<Vector3> normals;
    std::vector<GLuint> indices;
};

void LoadMesh(Mesh *mesh, const char *path);
void UnloadMesh(Mesh *mesh);