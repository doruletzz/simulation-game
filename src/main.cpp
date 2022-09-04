#include <iostream>
#include <string>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;

// Vertex Shader
static const char *vShader = "                      \n\
#version 330                                        \n\
                                                    \n\
layout (location = 0) in vec3 pos;                  \n\
void main(){                                        \n\
    gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);   \n\
                                                    \n\
}                                                   \n\
";

static const char *fShader = "                      \n\
#version 330                                        \n\
                                                    \n\
out vec4 color;                                     \n\
void main(){                                        \n\
    color = vec4(1.0, 0.0, 0.0, 1.0);   \n\
                                                    \n\
}                                                   \n\
";

void AddShader(GLuint program, const char *shaderCode, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    const GLchar *code[1];

    code[0] = shaderCode;

    GLint length[1];
    length[0] = strlen(shaderCode);

    glShaderSource(shader, 1, code, length);
    glCompileShader(shader);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        std::cout << "ERROR COMPILING THE" << shaderType << " SHADER: " << eLog << "\n";
        return;
    }

    glAttachShader(program, shader);
}

void CompileShaders()
{
    shader = glCreateProgram();

    if (!shader)
    {
        std::cout << "ERROR CREATING SHADER PROGRAM";
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);

    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        std::cout << "ERROR LINKING PROGRAM: " << eLog << "\n";
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        std::cout << "ERROR VALIDATING PROGRAM: " << eLog << "\n";
        return;
    }
}

void CreateTriangle()
{
    GLfloat vertices[] = {
        -1.0f,
        -1.0f,
        0.0f,
        1.0f,
        -1.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

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

    CreateTriangle();
    CompileShaders();

    // loop until window closed
    while (!glfwWindowShouldClose(window))
    {
        // Get handle user inputs
        glfwPollEvents();

        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    return 0;
}