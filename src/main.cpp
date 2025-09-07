#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"

// Version
constexpr char Ver_num[] = "6.6";
constexpr char Ver_name[] = "Our own shader class";

// Temporal vertex shader source code
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos; // position has attribute position 0\n"
"layout (location = 1) in vec3 aColor; // color has attribute position 1\n"
"out vec3 ourColor; // output a color to the fragment shader\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"    ourColor = aColor; // set ourColor to input color from the vertex data\n"
"}\n";

// Temporal fragment shader source code
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(ourColor, 1.0);\n"
"}\n";

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
    const std::string windowTitle = std::string(Ver_num) + " - " + std::string(Ver_name);
    GLFWwindow* window = glfwCreateWindow(800, 600, windowTitle.c_str(), nullptr, nullptr);
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
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // top
    };

    unsigned int indices[] = {
        0, 1, 2
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
    // First attribute:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Second attribute:
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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
        glUseProgram(shaderProgram);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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