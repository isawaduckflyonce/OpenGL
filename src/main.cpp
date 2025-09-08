// Standard libraries
#include <iostream>

// Included libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Headers
#include "shader.h"


// Version
constexpr char VER_NUM[] = "9.8";
constexpr char VER_NAME[] = "More 3D";

const unsigned short int SCR_WIDTH = 800;
const unsigned short int SCR_HEIGHT = 600;

// Callback functions
void framebuffer_size_callback(GLFWwindow* window, const int width, const int height)
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
    const std::string windowTitle = std::string(VER_NUM) + " - " + std::string(VER_NAME);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, windowTitle.c_str(), nullptr, nullptr);
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

    constexpr float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    constexpr unsigned int indices[] = {

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
    // First attribute (positions):
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    // Second attribute (colors):
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    // Third attribute (texture coords):
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // 5. Generate and bind EBO (Element Buffer Object)
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 6. Unbind the VAO, VBO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // 1. Generate textures
    unsigned int textures[2];
    glGenTextures(2, textures);

    // 2. Load and generate texture data
    stbi_set_flip_vertically_on_load(true); // So images don't render upside down
    int width[2], height[2], nrChannels[2];
    unsigned char *data[2] = {
        stbi_load("src/textures/prototype_textures/red/texture_01.png", &width[0], &height[0], &nrChannels[0], 0),
        stbi_load("src/textures/sneaky_golem.jpg", &width[1], &height[1], &nrChannels[1], 0)
    };

    // 3. Activate and bind texture 1
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    // 4. Set texture 1 parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 5. Use the texture 1
    if (data[0])
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[0], height[0], 0, GL_RGB, GL_UNSIGNED_BYTE, data[0]);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture 1" << std::endl;
    }

    // 6. Free image 1's memory
    stbi_image_free(data[0]);

    // 7. Activate and bind texture 2
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    // 8. Set texture 2 parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 9. Use the texture 2
    if (data[1])
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // To fix skewing
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[1], height[1], 0, GL_RGB, GL_UNSIGNED_BYTE, data[1]);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture 2" << std::endl;
    }

    // 10. Free image 2's memory
    stbi_image_free(data[1]);

    // Setup shader
    Shader customShader("src/shaders/default/default.vert", "src/shaders/default/default.frag");
    customShader.use();
    customShader.setInt("ourTexture1", 0);
    customShader.setInt("ourTexture2", 1);


    // 1. Model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    // Model transformations:
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -2.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));

    // 2. View matrix
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    // World/Camera transformations:
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -10.0f));

    // 3. Perspective / Ortho projection matrix
    glm::mat4 perspMatrix = glm::mat4(1.0f);
    perspMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 orthoMatrix = glm::mat4(1.0f);
    orthoMatrix = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, 0.0f, 100.0f);

    // Getting uniforms' locations
    unsigned int modelMatLoc = glGetUniformLocation(customShader.ID,"model");
    unsigned int viewMatLoc = glGetUniformLocation(customShader.ID,"view");
    unsigned int projMatLoc = glGetUniformLocation(customShader.ID,"projection");

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
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);

        // Transform cube
        modelMatrix = glm::rotate(modelMatrix, glm::radians(-1.0f), glm::vec3(0.0, 1.0, 0.0));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(0.1f), glm::vec3(1.0, 0.0, 0.0));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(0.5f), glm::vec3(0.0, 0.0, 1.0));


        customShader.setFloat("alpha", pow(sinf(glfwGetTime()), 2));

        // Apply matrix transformations
        glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(perspMatrix));

        // Draw model
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Call events and swap buffer
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(2, textures);
    //glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}