cmake_minimum_required(VERSION 3.6)
project(semestralka)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-modules/")
set(CMAKE_CXX_FLAGS "-std=c++1z")

# Add libraries
find_package(OpenGL  REQUIRED)
find_package(GLEW    REQUIRED STATIC)
find_package(glfw3   REQUIRED)
find_package(glm     REQUIRED)
find_package(Boost   REQUIRED COMPONENTS system filesystem)
find_package(fmt     REQUIRED)
find_package(YamlCpp REQUIRED)

# Add GSL
add_library(gsl INTERFACE)

target_include_directories(gsl INTERFACE gsl-lite/include/)

# Add ImGui
add_library(
        imgui STATIC
        imgui/src/imgui.cpp
        imgui/src/imgui_draw.cpp
        imgui/src/imgui_demo.cpp
        imgui/src/imgui_impl_glfw_gl3.cpp
)

target_include_directories(imgui PUBLIC imgui/inc/ ${GLEW_INCLUDE_DIRS})

# Add SOIL
add_library(soil STATIC
        SOIL/src/image_helper.c
        SOIL/src/stb_image_aug.c
        SOIL/src/image_DXT.c
        SOIL/src/SOIL.c
)

target_include_directories(soil PUBLIC SOIL/src/)

if (APPLE)
    target_link_libraries(soil "-framework CoreFoundation")
endif ()

# Add targets
set(SOURCE_FILES src/main.cpp src/Program.cpp src/Program.h src/model.cpp src/model.h src/Camera.cpp src/Camera.h src/Light.cpp src/Light.h src/GLObject.cpp src/GLObject.h src/glmUniforms.cpp src/Material.cpp src/Material.h src/Texture.cpp src/Texture.h src/geometry.cpp src/geometry.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(
        ${PROJECT_NAME} PUBLIC
        ${OPENGL_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        # ${GLFW_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
        ${fmt_INCLUDE_DIRS}
        ${YAMLCPP_INCLUDE_DIR}
        tinyobjloader/
)

target_link_libraries(
        ${PROJECT_NAME} PUBLIC
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        glfw
        ${GLM_LIBRARIES}
        ${Boost_LIBRARIES}
        ${YAMLCPP_LIBRARY}
        fmt
        imgui
        soil
        gsl
)

# Copy resources
add_custom_target(
        copy_shaders
        ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shd
        ${CMAKE_CURRENT_BINARY_DIR}/shd
        COMMENT "Copying shaders" VERBATIM
)

add_dependencies(${PROJECT_NAME} copy_shaders)

add_custom_target(
        copy_resources
        ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/res
        ${CMAKE_CURRENT_BINARY_DIR}/res
        COMMENT "Copying resources" VERBATIM
)

add_dependencies(${PROJECT_NAME} copy_resources)
