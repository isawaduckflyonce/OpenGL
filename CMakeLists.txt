cmake_minimum_required(VERSION 3.10)
project(OpenGL)

set(CMAKE_CXX_STANDARD 17)

# Tell CMake to prefer static linking for all libraries in this project.
set(BUILD_SHARED_LIBS OFF)

# Add the GLFW source directory. The path is relative to the current CMakeLists.txt file.
add_subdirectory(lib/glfw)

# Find X11 and OpenGL as they are dependencies.
find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)

# Create a static library from the GLAD source file
add_library(glad STATIC lib/glad/src/glad.c)
target_include_directories(glad PUBLIC lib/glad/include)

# Add your executable target, specifying the correct path to your source file.
add_executable(OpenGL src/main.cpp)

# Link your executable to the necessary static libraries.
target_link_libraries(OpenGL
        PRIVATE
        glfw
        glad
        ${X11_LIBRARIES}
        ${OPENGL_LIBRARIES}
)

# Set the include directories for your main executable
target_include_directories(OpenGL
        PRIVATE
        lib/glad/include
        ${glfw_SOURCE_DIR}/include
)