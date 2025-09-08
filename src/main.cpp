#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

GLFWwindow *window;
float r = 239.0f / 255.0f;
float g = 136.0f / 255.0f;
float b = 190.0f / 255.0f;
float a = 1.0f;

int init()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //Request OpenGL 4.6 (1.0 loaded by default)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //Load OpenGL extensions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return -1;
    
    return 0; //Success
}
void update()
{
    /* Render here */
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
}

int main(void)
{
    init();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
        update();

    glfwTerminate();
    return 0;
}