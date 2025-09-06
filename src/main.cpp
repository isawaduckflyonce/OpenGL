#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include "GLFW/glfw3.h"

// Version
constexpr char Ver_num[] = "6.4";
constexpr char Ver_name[] = "Uniforms";

// Temporal vertex shader source code
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos, 1.0);\n"
" vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
"}\0";

// Temporal fragment shader source code
const char *fragmentShaderSource = "#version 330 core\n"
"uniform vec4 ourColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = ourColor;\n"
"}\0";

// Callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Input processing
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    // GLFW initialization:
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Major OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Minor OpenGL version
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creating window object:
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLAD initialization
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Default viewport initialization
    glViewport(0, 0, 800, 600);

    // Registering callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /////////////////////////////////////////

    float vertices[] = {
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f // top left
        };
    unsigned int indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
        };

    /////////////////////////////////////////

    // 1. Generate and bind the VAO (Vertex Array Object)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // 2. Generate and bind the VBO (Vertex Buffer Object)
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 3. Copy vertex data into the VBO:
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 4. Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 5. Generate and bind EBO (Element Buffer Object)
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 6. Unbind the VAO, VBO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Vertex shader creation
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Vertex shader source code attachment and compilation
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Checking for vertex shader compilation errors
    int VSsuccess;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &VSsuccess);
    if(!VSsuccess)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
        infoLog << std::endl;

        return -1;
    }


    // Fragment shader creation
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Fragment shader source code attachment and compilation
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Checking for fragment shader compilation errors
    int FSsuccess;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &FSsuccess);
    if(!FSsuccess)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
        infoLog << std::endl;

        return -1;
    }


    // Shader program creation
    unsigned int shaderProgram = glCreateProgram();

    // Shader attachment
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Checking for linking errors
    int Lsuccess;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &Lsuccess);
    if(!Lsuccess) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
        infoLog << std::endl;

        return -1;
    }

    // Delete shader objects after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glBindVertexArray(VAO);

    // Render loop
    while(!glfwWindowShouldClose(window))
    {
        // Handle user input
        processInput(window);

        // Render commands
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // To draw, we use the shader program (VAO is already binded)
        const auto timeValue = (float)glfwGetTime();
        const float greenValue = (sinf(3*timeValue) / 2.0f) + 0.5f;
        const int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUseProgram(shaderProgram);
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // Call events and swap buffer
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}