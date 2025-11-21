#include "Shader.h"
#include "Window.h"
#include "raymath.h"
#include <cstddef>
// #include <cstdio>
// #include <cstdlib>

struct Vertex
{
    Vector2 pos; // offset of 0
    Vector3 col; // offset of 8 (4 bytes for pos.x + 4 bytes for pos.y = 8)
};

static const Vertex vertices_white[] =
    {
        {{-0.6f, -0.4f}, {1.0f, 1.0f, 1.0f}},
        { {0.6f, -0.4f}, {1.0f, 1.0f, 1.0f}},
        {   {0.f, 0.6f}, {1.0f, 1.0f, 1.0f}}
};
static const Vector2 vertex_positions[] =
    {
        {-0.6f, -0.4f},
        { 0.6f, -0.4f},
        {  0.f,  0.6f}
};
static const Vector3 vertex_colors[] =
    {
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}
};
static const Vertex vertices_quad[] =
    {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        { {0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}},
        {  {0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}},
        { {-0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}}
};
static int vertices_quad_indices[] =
    {
        0, 1, 2,
        0, 2, 3};
static int vertices_quad_indices2[] =
    {
        0, 1, 2,
        0, 3, 2};

int main()
{
    InitWindow(800, 800, "Graphics Course");

    // Colors are represented as fractions between 0.0 and 1.0, so convert using a colour-picker tool accordingly!
    float r = 239.0f / 255.0f;
    float g = 136.0f / 255.0f;
    float b = 190.0f / 255.0f;
    float a = 1.0f;

    GLuint a1_tri_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/a1_triangle.vert");
    GLuint a1_tri_frag = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/a1_triangle.frag");
    GLuint a1_tri_shader = CreateProgram(a1_tri_vert, a1_tri_frag);

    GLuint vbos_rainbow[2];
    glGenBuffers(2, vbos_rainbow);
    GLuint vertex_array_rainbow;
    glGenVertexArrays(1, &vertex_array_rainbow);
    glBindVertexArray(vertex_array_rainbow);

    glBindBuffer(GL_ARRAY_BUFFER, vbos_rainbow[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbos_rainbow[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_colors), vertex_colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //--------------------------------------------

    GLuint vertex_buffer_white;
    glGenBuffers(1, &vertex_buffer_white);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_white);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_white), vertices_white, GL_STATIC_DRAW);

    GLuint vertex_array_white;
    glGenVertexArrays(1, &vertex_array_white);
    glBindVertexArray(vertex_array_white);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, col));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //--------------------------------------------

    GLuint vbo_quad;
    glGenBuffers(1, &vbo_quad);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_quad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_quad), vertices_quad, GL_STATIC_DRAW);

    GLuint ebo_quad;
    glGenBuffers(1, &ebo_quad);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_quad);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices_quad_indices), vertices_quad_indices, GL_STATIC_DRAW);

    GLuint vao_quad;
    glGenVertexArrays(1, &vao_quad);
    glBindVertexArray(vao_quad);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, col));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //--------------------------------------------

    GLuint ebo_quad2;
    glGenBuffers(1, &ebo_quad2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_quad2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices_quad_indices2), vertices_quad_indices2, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //--------------------------------------------

    int object_index = 0;
    int total_cases = 7;
    glEnable(GL_CULL_FACE); //by default this is disabled
    bool is_ccw = true; //opengl's default is also ccw
    bool is_culling_back = true; //opengl's default is also culling the back face

    GLint u_color = glGetUniformLocation(a1_tri_shader, "u_color");
    GLint u_world = glGetUniformLocation(a1_tri_shader, "u_world");

    Matrix world = MatrixIdentity();

    // Generally you want to Scale * Rotate * Translate (order matters)!!!
    //world = MatrixRotateZ(30.0f * DEG2RAD) * MatrixTranslate(0.5f, 0.0f, 0.0f);

    //--------------------------------------------

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE))
            SetWindowShouldClose(true);

        // Time in seconds since GLFW was initialized (use this with functions like sinf and cosf for repeating animations)
        float tt = Time();

        /* Render here */
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);

        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_UP))
        {
            ++object_index %= total_cases;
        }
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_DOWN))
        {
            --object_index %= total_cases;
            if (object_index < 0)
                object_index += total_cases;
        }
        if (IsKeyPressed(KEY_Q))
        {
            is_ccw = !is_ccw;
            glFrontFace(is_ccw ? GL_CCW : GL_CW);
        }
        if (IsKeyPressed(KEY_W))
        {
            is_culling_back = !is_culling_back;
            glCullFace(is_culling_back ? GL_BACK : GL_FRONT);
        }

        switch (object_index)
        {
        case 0:
            //white triangle
            glUseProgram(a1_tri_shader);
            glUniform3f(u_color, 1.0f, 1.0f, 1.0f);
            //must use MatrixToFloat to send mat4 since raylib matrix memory is in a different layout than glsl mat4
            glUniformMatrix4fv(u_world, 1, GL_FALSE, MatrixToFloat(MatrixIdentity()));
            glBindVertexArray(vertex_array_white);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            break;

        case 1:
            //rainbow triangle
            glUseProgram(a1_tri_shader);
            glUniform3f(u_color, 1.0f, 1.0f, 1.0f);
            glUniformMatrix4fv(u_world, 1, GL_FALSE, MatrixToFloat(MatrixIdentity()));
            glBindVertexArray(vertex_array_rainbow);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            break;

        case 2:
            //colour changes over time (using uniforms)
            {
                float r = (sinf(tt * 2.0f) + 1.0f) * 0.5f;
                float g = (sinf(tt * 2.0f + 2.094f) + 1.0f) * 0.5f; // 2π/3 phase offset
                float b = (sinf(tt * 2.0f + 4.188f) + 1.0f) * 0.5f; // 4π/3 phase offset
                glUseProgram(a1_tri_shader);
                glUniform3f(u_color, r, g, b);
                glUniformMatrix4fv(u_world, 1, GL_FALSE, MatrixToFloat(MatrixIdentity()));
                glBindVertexArray(vertex_array_white);
                glDrawArrays(GL_TRIANGLES, 0, 3);
            }
            break;

        case 3:
            //translates back and forth from x = 1 to x = -1
            {
                float translateX = sinf(tt);
                world = MatrixTranslate(translateX, 0.0f, 0.0f);
                glUseProgram(a1_tri_shader);
                glUniform3f(u_color, 1.0f, 1.0f, 1.0f);
                glUniformMatrix4fv(u_world, 1, GL_FALSE, MatrixToFloat(world));
                glBindVertexArray(vertex_array_rainbow);
                glDrawArrays(GL_TRIANGLES, 0, 3);
            }
            break;

        case 4:
            //rotates counter-clockwise about the z-axis
            {
                world = MatrixRotateZ(tt);
                glUseProgram(a1_tri_shader);
                glUniform3f(u_color, 1.0f, 1.0f, 1.0f);
                glUniformMatrix4fv(u_world, 1, GL_FALSE, MatrixToFloat(world));
                glBindVertexArray(vertex_array_rainbow);
                glDrawArrays(GL_TRIANGLES, 0, 3);
            }
            break;
        case 5:
            glUseProgram(a1_tri_shader);
            glUniform3f(u_color, 1.0f, 1.0f, 1.0f);
            glUniformMatrix4fv(u_world, 1, GL_FALSE, MatrixToFloat(MatrixIdentity()));
            glBindVertexArray(vao_quad);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_quad);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            break;
        case 6:
            glUseProgram(a1_tri_shader);
            glUniform3f(u_color, 1.0f, 1.0f, 1.0f);
            glUniformMatrix4fv(u_world, 1, GL_FALSE, MatrixToFloat(MatrixIdentity()));
            glBindVertexArray(vao_quad);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_quad2);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            break;
        }

        // Called at end of the frame to swap buffers and update input
        UpdateWindow();
    }

    glDeleteVertexArrays(1, &vertex_array_rainbow);
    glDeleteVertexArrays(1, &vertex_array_white);
    glDeleteVertexArrays(1, &vao_quad);
    glDeleteBuffers(2, vbos_rainbow);
    glDeleteBuffers(1, &vertex_buffer_white);
    glDeleteBuffers(1, &vbo_quad);
    glDeleteBuffers(1, &ebo_quad);
    glDeleteBuffers(1, &ebo_quad2);
    glDeleteProgram(a1_tri_shader);
    glDeleteShader(a1_tri_frag);
    glDeleteShader(a1_tri_vert);

    KillWindow();
    return 0;
}