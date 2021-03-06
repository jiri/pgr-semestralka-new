#include <iostream>
#include <array>
using namespace std;

#include <gsl.h>

#define TINYOBJLOADER_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#define GLEW_STATIC

#include <GLFW/glfw3.h>
#include <GL/glew.h>

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <tiny_obj_loader.h>
#include <yaml-cpp/yaml.h>

#include "Program.h"
#include "Camera.h"
#include "model.h"
#include "Light.h"

void error_callback(int /* error */, const char *message) {
    cerr << "GLFW error: " << message << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
}

struct AppState {
    Camera camera;
    array<bool, 1024> keys;
    bool cameraActive;
};

void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods) {
    auto &as = *static_cast<AppState*>(glfwGetWindowUserPointer(w));

    if (action == GLFW_PRESS) {
        as.keys[key] = true;

        if (key == GLFW_KEY_TAB) {
            as.cameraActive = !as.cameraActive;
        }
    } else if (action == GLFW_RELEASE) {
        as.keys[key] = false;
    }

    if (as.cameraActive) {
        glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if (!as.cameraActive) {
        ImGui_ImplGlfwGL3_KeyCallback(w, key, scancode, action, mods);
    }
}

void pos_callback(GLFWwindow *w, double x, double y) {
    static double last_x = x;
    static double last_y = y;

    GLfloat dx = (GLfloat) (x - last_x);
    GLfloat dy = (GLfloat) (y - last_y);

    last_x = x;
    last_y = y;

    auto& as = *static_cast<AppState *>(glfwGetWindowUserPointer(w));

    if (as.cameraActive) {
        as.camera.yaw   += dx * 0.1f;
        as.camera.pitch -= dy * 0.1f;

        if (as.camera.pitch > 89.0f) {
            as.camera.pitch = 89.0f;
        }
        if (as.camera.pitch < -89.0f) {
            as.camera.pitch = -89.0f;
        }
    }
}

int main() {
    /* Create a window */
    glfwSetErrorCallback(error_callback);

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,        GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE,             GL_FALSE);
    glfwWindowHint(GLFW_FOCUSED,               GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES,               2);

    auto window = glfwCreateWindow(800, 600, "", nullptr, nullptr);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, pos_callback);
    glfwSetMouseButtonCallback(window, ImGui_ImplGlfwGL3_MouseButtonCallback);
    glfwSetScrollCallback(window, ImGui_ImplGlfwGL3_ScrollCallback);
    glfwSetCharCallback(window, ImGui_ImplGlfwGL3_CharCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* Initialize AppState */
    auto as = AppState {
        .camera = { vec3(0.0f, 3.0f, 3.0f), vec3(0.0f, 0.0f, 0.0f) },
        .keys = { GL_FALSE },
        .cameraActive = GL_TRUE,
    };

    glfwSetWindowUserPointer(window, &as);

    /* Initialize ImGui */
    ImGui_ImplGlfwGL3_Init(window, false);

    /* Initialize OpenGL */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); /* Turn on VSync */

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cerr << "Failed to initialize GLEW" << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    YAML::Node config = YAML::LoadFile("config.yml");

    /* Load shaders */
    Program phong { "shd/phong.vert", "shd/phong.frag" };
    Program lamp  { "shd/light.vert", "shd/light.frag" };

    /* Load data */
    auto cube = model { "res/chalet2.obj" };
    auto cubeModel = scale(vec3 { 1.0f }) * rotate(radians(-180.0f), vec3 { 0.0f, 1.0f, 0.0f });

    auto light = Light { vec3 { 1.0f, 1.0f, 0.7f } };
    auto lightModel = mat4 { 1.0f };

    /* Matrices */
    mat4 projection = perspective(radians(65.0f), 4.0f/ 3.0f, 0.01f, 100.0f);

    vec3 pos = vec3 { 4.0f, 0.0f, 0.0f };

    while (!glfwWindowShouldClose(window)) {
        /* Handle input */
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        if (as.cameraActive) {
            as.camera.handleInput(as.keys);
        }

        /* FPS counter */
        ImGui::Begin("", nullptr, ImVec2 { 0.0f, 0.0f }, 0.0f, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoInputs);
            ImGui::Text("%.0f FPS", ImGui::GetIO().Framerate);
        ImGui::End();

        /* Manipulation */
        ImGui::Begin("Editor");
            ImGui::SliderFloat3("Light position", value_ptr(pos), -10.0f, 10.0f);
        ImGui::End();

        lightModel = translate(pos) * scale(vec3 { 0.25f });

        /* Render */
        glClearColor(0.059f, 0.057f, 0.073f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        {
            auto handle = cube.material.program->use();

            handle.setUniform("projection", projection);
            handle.setUniform("view", as.camera.viewMatrix());
            handle.setUniform("model", cubeModel);

            handle.setUniform("light", light);
            handle.setUniform("light.position", vec3 { lightModel * vec4 { 0.0f, 0.0f, 0.0f, 1.0f } });

            handle.setUniform("cameraPosition", as.camera.position);

            handle.setUniform("material", cube.material);

            cube.draw();
        }

        {
            auto handle = lamp.use();

            handle.setUniform("projection", projection);
            handle.setUniform("view", as.camera.viewMatrix());
            handle.setUniform("model", lightModel);

            handle.setUniform("light", light);
            handle.setUniform("light.position", vec3 { lightModel * vec4 { 0.0f, 0.0f, 0.0f, 1.0f } });

            handle.setUniform("cameraPosition", as.camera.position);

            light.draw();
        }

        /* Present */
        ImGui::Render();

        glfwSwapBuffers(window);
    }

    /* Cleanup and exit */
    glfwTerminate();
    ImGui_ImplGlfwGL3_Shutdown();

    return 0;
}
