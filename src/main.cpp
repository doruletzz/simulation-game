#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

int main()
{
    if (!glfwInit())
    {
        std::cout << "INIT FAILED...";
        glfwTerminate();
        return 1;
    }

    // Setup GLFW windows props
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // init window
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "TEST WINDOW", NULL, NULL);

    if (!window)
    {
        std::cout << "GLFWWINDOW CREATION FAILED...";
        glfwTerminate();
        return 1;
    }

    // get buffer size info
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    // Set context for glew
    glfwMakeContextCurrent(window);

    // Allow modern extensions (if needed)
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW INIT FAILED";
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // Setup viewport size;
    glViewport(0, 0, bufferWidth, bufferHeight);

    // loop until window closed
    while (!glfwWindowShouldClose(window))
    {
        // Get handle user inputs
        glfwPollEvents();

        // Clear window
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    return 0;
}