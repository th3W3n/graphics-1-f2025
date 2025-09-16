#define GLFW_INCLUDE_NONE
#include "Window.h"
#include <GLFW/glfw3.h>
#include <cassert>
#include <cstdio>
#include <glad/glad.h>
#include <memory>

struct App
{
    GLFWwindow *window = nullptr;
    int keysPrevAction[KEY_COUNT]{};
    int keysCurrAction[KEY_COUNT]{};
} gApp;

void KeyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_REPEAT)
        return;
    gApp.keysCurrAction[key] = action;

    // //uncomment to see how key events work!
    // const char *name = glfwGetKeyName(key, scancode);
    // if (action == GLFW_PRESS)
    //     printf("%s is down\n", name);
    // else if (action == GLFW_RELEASE)
    //     printf("%s is up\n", name);
}

void InitWindow(int width, int height, const char *title)
{
    /* Initialize the library */
    assert(glfwInit() == GLFW_TRUE);

    //Request OpenGL 4.6 (1.0 loaded by default)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    gApp.window = glfwCreateWindow(width, height, title, NULL, NULL);
    assert(gApp.window != nullptr);

    /* Make the window's context current */
    glfwMakeContextCurrent(gApp.window);

    //Load OpenGL extensions
    assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));

    glfwSetKeyCallback(gApp.window, KeyboardCallback);
}

void SetWindowShouldClose(bool close)
{
    glfwSetWindowShouldClose(gApp.window, close ? GLFW_TRUE : GLFW_FALSE);
}
bool WindowShouldClose()
{
    return glfwWindowShouldClose(gApp.window);
}

void UpdateWindow()
{
    memcpy(gApp.keysPrevAction, gApp.keysCurrAction, sizeof(int) * KEY_COUNT);
    /* Swap front and back buffers */
    glfwSwapBuffers(gApp.window);
    /* Poll for and process events */
    glfwPollEvents();
}

bool IsKeyDown(int key)
{
    return gApp.keysCurrAction[key] == GLFW_PRESS;
}
bool IsKeyUp(int key)
{
    return gApp.keysCurrAction[key] == GLFW_RELEASE;
}
bool IsKeyPressed(int key)
{
    return gApp.keysPrevAction[key] == GLFW_PRESS && gApp.keysCurrAction[key] == GLFW_RELEASE;
}

void KillWindow()
{
    glfwTerminate();
}