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
constexpr char VER_NUM[] = "10.8";
constexpr char VER_NAME[] = "Zoom";

// Window dimensions
constexpr unsigned short int SCR_WIDTH = 800;
constexpr unsigned short int SCR_HEIGHT = 600;

// Camera
glm::vec3 cameraPos, cameraFront, cameraUp;
float pitch = 0.0f;
float yaw = -90.0f;;
float mouseLastX = SCR_WIDTH / 2.0f;
float mouseLastY = SCR_HEIGHT / 2.0f;
float sensitivity = 0.1f;
bool firstMouse = true;
float fov = 45.0f;
float zoom = 0.0f;

// Delta time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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

    const float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *
        cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *
        cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPos += cameraSpeed * -cameraUp;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        mouseLastX = (float)xpos;
        mouseLastY = (float)ypos;
        firstMouse = false;
    }

    // 1. Calculate the mouse’s offset since the last frame.
    float xoffset = (float)xpos - mouseLastX;
    float yoffset = mouseLastY - (float)ypos;
    mouseLastX = (float)xpos;
    mouseLastY = (float)ypos;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    // 2. Add the offset values to the camera’s yaw and pitch values.
    yaw += xoffset;
    pitch += yoffset;

    // 3. Add some constraints to the minimum/maximum pitch values.
    pitch = pitch > 89.0f ? 89.0f : pitch;
    pitch = pitch < -89.0f ? -89.0f : pitch;

    // 4. Calculate the direction vector.
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    zoom -= (float)yoffset;
    zoom = zoom > 45.0f ? 45.0f : zoom;
    zoom = zoom < -45.0f ? -45.0f : zoom;
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
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Registering callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Window input mode
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    constexpr unsigned int indices[] = {};

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
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


    // 1. Model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    // Model transformations:
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -2.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));

    // 2. View matrix
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    // World/Camera transformations:
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -5.0f));

    // 3. Perspective / Ortho projection matrix
    glm::mat4 perspMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 orthoMatrix = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, 0.0f, 100.0f);


    // Camera setup
    cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // "Look at" matrix
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    viewMatrix = view;


    // Setup shader
    Shader customShader("src/shaders/default/default.vert", "src/shaders/default/default.frag");
    customShader.use();
    customShader.setInt("ourTexture1", 0);
    customShader.setInt("ourTexture2", 1);

    // Getting uniforms' locations
    unsigned int modelMatLoc = glGetUniformLocation(customShader.ID,"model");
    unsigned int viewMatLoc = glGetUniformLocation(customShader.ID,"view");
    unsigned int projMatLoc = glGetUniformLocation(customShader.ID,"projection");


    // Enable Z-Buffer
    glEnable(GL_DEPTH_TEST);

    // Bind VAO
    glBindVertexArray(VAO);

    // To draw, we use the shader program (VAO is already binded)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    // Render loop
    while(!glfwWindowShouldClose(window))
    {
        // Handle user input
        processInput(window);

        // Render commands
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        // Clear screen and Z-Buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Camera
        glfwSetCursorPosCallback(window, mouse_callback);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glfwSetScrollCallback(window, scroll_callback);
        perspMatrix = glm::perspective(glm::radians(fov + zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        viewMatrix = view;


        // Apply matrix transformations
        glUniformMatrix4fv((int)modelMatLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv((int)viewMatLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv((int)projMatLoc, 1, GL_FALSE, glm::value_ptr(perspMatrix));


        for (unsigned int i = 0; i < std::size(cubePositions); i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);

            // Transform cube
            model = glm::rotate(model, glm::radians(20 * (float)glfwGetTime() + (float)(i * i)), glm::vec3(0.0, 1.0, 0.0));
            model = glm::rotate(model, glm::radians(20 * (float)glfwGetTime() + (float)(i * i)), glm::vec3(1.0, 0.0, 0.0));
            model = glm::rotate(model, glm::radians(20 * (float)glfwGetTime() + (float)(i * i)), glm::vec3(0.0, 0.0, 1.0));

            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

            // Set shader uniforms
            customShader.setFloat("alpha", (float)pow(sinf((float)glfwGetTime() + 100.0f * (float)i), 2));

            glUniformMatrix4fv((int)modelMatLoc, 1, GL_FALSE, glm::value_ptr(model));

            // Draw models
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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