#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "raymath.h"
#include <cstddef>
#include <cstdlib>
#include <vector>

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
static const GLuint vertices_quad_indices[] =
    {
        0, 1, 2,
        0, 2, 3};
static const GLuint vertices_quad_indices2[] =
    {
        0, 1, 2,
        0, 3, 2};
static Vector3 rgb_frequency = {0.0f, 0.0f, 0.0f};
static Vector3 rgb_color = {0.0f, 0.0f, 0.0f};
static bool isRandValChanged = false;

static const Vector3 new_positions[] =
    {
        {-0.5f, -0.5f, 0.0f}, //bottom-left
        { 0.5f, -0.5f, 0.0f}, //bottom-right
        { 0.5f,  0.5f, 0.0f}, //top-right
        {-0.5f,  0.5f, 0.0f}  //top-left
};
static const Vector2 new_tcoords[] =
    {
        {0.0f, 0.0f}, //bottom-left
        {1.0f, 0.0f}, //bottom-right
        {1.0f, 1.0f}, //top-right
        {0.0f, 1.0f}  //top-left
};
static const Vector3 new_normals[] =
    {
        Vector3UnitZ,
        Vector3UnitZ,
        Vector3UnitZ,
        Vector3UnitZ};

int main()
{
    std::vector<GLuint> handles_buffers;
    std::vector<GLuint> handles_vaos;

    InitWindow(800, 800, "Graphics Course");
    float r, g, b, a;
    r = g = b = a = 0.0f;

    GLuint shader1_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/shader1.vert");
    GLuint shader1_frag = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/shader1.frag");
    GLuint shader1 = CreateProgram(shader1_vert, shader1_frag);

    GLuint shader2_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/shader2.vert");
    GLuint shader2_frag = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/shader2.frag");
    GLuint shader2 = CreateProgram(shader2_vert, shader2_frag);

    GLuint vbos_rainbow[2];
    glGenBuffers(2, vbos_rainbow);
    handles_buffers.push_back(vbos_rainbow[0]);
    handles_buffers.push_back(vbos_rainbow[1]);
    GLuint vertex_array_rainbow;
    glGenVertexArrays(1, &vertex_array_rainbow);
    handles_vaos.push_back(vertex_array_rainbow);
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

    //case 0--------------------------------------------

    GLuint vertex_buffer_white;
    glGenBuffers(1, &vertex_buffer_white);
    handles_buffers.push_back(vertex_buffer_white);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_white);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_white), vertices_white, GL_STATIC_DRAW);

    GLuint vertex_array_white;
    glGenVertexArrays(1, &vertex_array_white);
    handles_vaos.push_back(vertex_array_white);
    glBindVertexArray(vertex_array_white);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, col));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //case 5--------------------------------------------

    GLuint vbo_quad;
    glGenBuffers(1, &vbo_quad);
    handles_buffers.push_back(vbo_quad);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_quad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_quad), vertices_quad, GL_STATIC_DRAW);

    GLuint ebo_quad;
    glGenBuffers(1, &ebo_quad);
    handles_buffers.push_back(ebo_quad);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_quad);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices_quad_indices), vertices_quad_indices, GL_STATIC_DRAW);

    GLuint vao_quad;
    glGenVertexArrays(1, &vao_quad);
    handles_vaos.push_back(vao_quad);
    glBindVertexArray(vao_quad);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, col));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //case 6--------------------------------------------

    GLuint ebo_quad2;
    glGenBuffers(1, &ebo_quad2);
    handles_buffers.push_back(ebo_quad2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_quad2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices_quad_indices2), vertices_quad_indices2, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //case 7--------------------------------------------

    GLuint vbo_new_positions;
    GLuint vbo_new_tcoords;
    GLuint vbo_new_normals;
    glGenBuffers(1, &vbo_new_positions);
    glGenBuffers(1, &vbo_new_tcoords);
    glGenBuffers(1, &vbo_new_normals);
    handles_buffers.push_back(vbo_new_positions);
    handles_buffers.push_back(vbo_new_tcoords);
    handles_buffers.push_back(vbo_new_normals);
    GLuint vao_new;
    glGenVertexArrays(1, &vao_new);
    handles_vaos.push_back(vao_new);
    glBindVertexArray(vao_new);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_new_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(new_positions), new_positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_new_tcoords);
    glBufferData(GL_ARRAY_BUFFER, sizeof(new_tcoords), new_tcoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (GLvoid *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_new_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(new_normals), new_normals, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (GLvoid *)0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //case 8--------------------------------------------

    Mesh plane;
    CreateMesh(&plane, 4, 6, new_positions, new_tcoords, new_normals, vertices_quad_indices);
    LoadMesh(&plane);

    //case 9--------------------------------------------

    // Mesh head;
    // LoadMesh(&head, "./assets/meshes/Monkey.obj");
    Mesh par;
    LoadMesh(&par, ParShapesType::MESH_OCTAHEDRON);

    //===============================================================================
    //===============================================================================

    int object_index = 0;
    int total_cases = 10;
    glEnable(GL_CULL_FACE); //by default this is disabled
    bool is_ccw = true; //opengl's default is also ccw
    bool is_culling_back = true; //opengl's default is also culling the back face

    GLint u_color = glGetUniformLocation(shader1, "u_color");
    GLint u_world = glGetUniformLocation(shader1, "u_world");
    GLint u_mvp = glGetUniformLocation(shader2, "u_mvp");

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

        if (object_index == 2)
        {
            if (!isRandValChanged)
            {
                float frequency = 3.0f;
                rgb_frequency.x = (float)rand() / RAND_MAX * frequency; //0 ~ frequency
                rgb_frequency.y = (float)rand() / RAND_MAX * frequency;
                rgb_frequency.z = (float)rand() / RAND_MAX * frequency;
                isRandValChanged = true;
            }
        }
        else
        {
            isRandValChanged = false;
        }
        switch (object_index)
        {
        case 0:
            //white triangle
            glUseProgram(shader1);
            glUniform3f(u_color, 1.0f, 1.0f, 1.0f);
            //must use MatrixToFloat to send mat4 since raylib matrix memory is in a different layout than glsl mat4
            glUniformMatrix4fv(u_world, 1, GL_FALSE, MatrixToFloat(MatrixIdentity()));
            glBindVertexArray(vertex_array_white);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            break;

        case 1:
            //rainbow triangle
            glUseProgram(shader1);
            glUniform3f(u_color, 1.0f, 1.0f, 1.0f);
            glUniformMatrix4fv(u_world, 1, GL_FALSE, MatrixToFloat(MatrixIdentity()));
            glBindVertexArray(vertex_array_rainbow);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            break;

        case 2:
            //colour changes over time (using uniforms)
            {
                rgb_color.x = (sinf(tt * rgb_frequency.x) + 1.0f) * 0.5f;
                rgb_color.y = (sinf(tt * rgb_frequency.y) + 1.0f) * 0.5f;
                rgb_color.z = (sinf(tt * rgb_frequency.z) + 1.0f) * 0.5f;
                glUseProgram(shader1);
                glUniform3fv(u_color, 1, &rgb_color.x);
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
                glUseProgram(shader1);
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
                glUseProgram(shader1);
                glUniform3f(u_color, 1.0f, 1.0f, 1.0f);
                glUniformMatrix4fv(u_world, 1, GL_FALSE, MatrixToFloat(world));
                glBindVertexArray(vertex_array_rainbow);
                glDrawArrays(GL_TRIANGLES, 0, 3);
            }
            break;
        case 5:
            glUseProgram(shader1);
            glUniform3f(u_color, 1.0f, 1.0f, 1.0f);
            glUniformMatrix4fv(u_world, 1, GL_FALSE, MatrixToFloat(MatrixIdentity()));
            glBindVertexArray(vao_quad);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_quad);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            break;
        case 6:
            glUseProgram(shader1);
            glUniform3f(u_color, 1.0f, 1.0f, 1.0f);
            glUniformMatrix4fv(u_world, 1, GL_FALSE, MatrixToFloat(MatrixIdentity()));
            glBindVertexArray(vao_quad);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_quad2);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            break;
        case 7:
            glUseProgram(shader2);
            glUniformMatrix4fv(u_mvp, 1, GL_FALSE, MatrixToFloat(MatrixIdentity()));
            glBindVertexArray(vao_new);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_quad);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            break;
        case 8:
            world = MatrixRotateZ(tt);
            DrawMesh(plane, shader2, u_mvp, world);
            break;
        case 9:
            //translate some offset first, then start rotating
            world = MatrixMultiply(MatrixTranslate(0.0f, -0.5f, 0.0f), MatrixRotateY(tt));
            DrawMesh(par, shader2, u_mvp, world);
            break;
        }

        // Called at end of the frame to swap buffers and update input
        UpdateWindow();
    }

    if (!handles_buffers.empty())
        glDeleteBuffers(GLsizei(handles_buffers.size()), handles_buffers.data());
    if (!handles_vaos.empty())
        glDeleteVertexArrays(GLsizei(handles_vaos.size()), handles_vaos.data());

    glDeleteProgram(shader1);
    glDeleteProgram(shader2);
    glDeleteShader(shader1_frag);
    glDeleteShader(shader1_vert);
    glDeleteShader(shader2_frag);
    glDeleteShader(shader2_vert);

    UnloadMesh(&plane);
    UnloadMesh(&par);
    // UnloadMesh(&head);

    KillWindow();
    return 0;
}