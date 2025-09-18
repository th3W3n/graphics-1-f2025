#include "Shader.h"
#include "Window.h"
#include <cstddef>

struct vec2
{
    float x; //4 bytes
    float y; //4 bytes
};
struct vec3
{
    float x;
    float y;
    float z;
};
struct Vertex
{
    vec2 pos; //offset of 0
    vec3 col; //offset of 8 (4 bytes for x and 4 for y) //rgb colors
};

static const Vertex vertices_rainbow[3] =
    {
        {{-0.6f, -0.4f}, {1.0f, 0.0f, 0.0f}},
        { {0.6f, -0.4f}, {0.0f, 1.0f, 0.0f}},
        {  {0.0f, 0.6f}, {0.0f, 0.0f, 1.0f}}
};
static const Vertex vertices_white[3] =
    {
        {{-0.6f, -0.4f}, {1.0f, 1.0f, 1.0f}},
        { {0.6f, -0.4f}, {1.0f, 1.0f, 1.0f}},
        {  {0.0f, 0.6f}, {1.0f, 1.0f, 1.0f}}
};

int main(void)
{
    InitWindow(800, 800, "Graphics Course");

    float r = 180.0f / 255.0f;
    float g = 250.0f / 255.0f;
    float b = 210.0f / 255.0f;
    float a = 1.0f;

    GLuint a1_tri_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/a1_triangle.vert");
    GLuint a1_tri_frag = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/a1_triangle.frag");
    GLuint a1_tri_shader = CreateProgram(a1_tri_vert, a1_tri_frag);

    GLuint vertex_buffer_rainbow;
    glGenBuffers(1, &vertex_buffer_rainbow);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_rainbow);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_rainbow), vertices_rainbow, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE); //unbind the buffer
    //can only have 1 vertex buffer bound at a time, so must unbind to prevent overwriting it

    GLuint vertex_buffer_white;
    glGenBuffers(1, &vertex_buffer_white);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_white);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_white), vertices_white, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    GLuint vertex_array_rainbow;
    glGenVertexArrays(1, &vertex_array_rainbow);
    glBindVertexArray(vertex_array_rainbow);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_rainbow);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, col));
    glBindVertexArray(GL_NONE);

    GLuint vertex_array_white;
    glGenVertexArrays(1, &vertex_array_white);
    glBindVertexArray(vertex_array_white);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_white);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, col));
    glBindVertexArray(GL_NONE);

    int object_index = 0;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE))
            SetWindowShouldClose(true);

        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);

        if (IsKeyPressed(KEY_SPACE))
            ++object_index %= 5;
        switch (object_index)
        {
        case 0:
            glUseProgram(a1_tri_shader);
            glBindVertexArray(vertex_array_rainbow);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            break;
        case 1:
            break;
        case 2:
            glUseProgram(a1_tri_shader);
            glBindVertexArray(vertex_array_white);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            break;
        case 3:
            break;
        case 4:
            glUseProgram(a1_tri_shader);
            glBindVertexArray(vertex_array_rainbow);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            break;
        }

        UpdateWindow();
    }

    KillWindow();
    return 0;
}