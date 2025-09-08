cmake_minimum_required(VERSION 3.10)
project(OpenGL)

set(CMAKE_CXX_STANDARD 17)

# Tell CMake to prefer static linking for all libraries in this project.
set(BUILD_SHARED_LIBS OFF)

# Add the GLFW source directory. The path is relative to the current CMakeLists.txt file.
add_subdirectory(lib/glfw)

# Find X11 and OpenGL as they are dependencies.
set(OpenGL_GL_PREFERENCE GLVND)
find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)

# Create a static library from the GLAD source file
add_library(glad STATIC lib/glad/src/glad.c)
target_include_directories(glad PUBLIC lib/glad/include)

# Add your executable target, specifying only the source files.
# CMake will handle header dependencies automatically.
add_executable(OpenGL
        src/main.cpp
        src/lib/shader.cpp
)

# Link your executable to the necessary static libraries.
target_link_libraries(OpenGL
        PRIVATE
        glfw
        glad
        ${X11_LIBRARIES}
        ${OPENGL_LIBRARIES}
)

# Set the include directories for your main executable
# Only include the directories that contain header files needed for compilation.
target_include_directories(OpenGL
        PRIVATE
        lib/glad/include
        ${glfw_SOURCE_DIR}/include
        lib/glm
        lib/stb_image
        src/include
)