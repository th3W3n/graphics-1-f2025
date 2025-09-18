#include "Window.h"
#include <cstddef>
#include <glad/glad.h>

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

static const char *vertex_shader_text =
    "#version 330\n"
    "layout (location = 0) in vec2 vPos;\n"
    "layout (location = 1) in vec3 vCol;\n"
    "out vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(vPos, 0.0, 1.0);\n"
    "    color = vCol;\n"
    "}\n";

static const char *fragment_shader_text =
    "#version 330\n"
    "in vec3 color;\n"
    "out vec4 fragment;\n"
    "void main()\n"
    "{\n"
    "    fragment = vec4(color, 1.0);\n"
    "}\n";

int main(void)
{
    InitWindow(800, 800, "Graphics Course");

    float r = 180.0f / 255.0f;
    float g = 250.0f / 255.0f;
    float b = 210.0f / 255.0f;
    float a = 1.0f;

    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    const GLuint program = glCreateProgram(); //shader program object
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

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
            glUseProgram(program);
            glBindVertexArray(vertex_array_rainbow);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            break;
        case 1:
            break;
        case 2:
            glUseProgram(program);
            glBindVertexArray(vertex_array_white);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            break;
        case 3:
            break;
        case 4:
            glUseProgram(program);
            glBindVertexArray(vertex_array_rainbow);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            break;
        }

        UpdateWindow();
    }

    KillWindow();
    return 0;
}