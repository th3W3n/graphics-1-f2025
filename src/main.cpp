#include "Window.h"
#include <glad/glad.h>

int main(void)
{
    InitWindow(800, 800, "Graphics Course");

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE))
            SetWindowShouldClose(true);

        float r = 239.0f / 255.0f;
        float g = 136.0f / 255.0f;
        float b = 190.0f / 255.0f;
        float a = 1.0f;

        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);

        UpdateWindow();
    }

    KillWindow();
    return 0;
}