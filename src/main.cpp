#include <iostream>
#include <vector>
using namespace std;

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "Program.h"
#include "Camera.h"
#include "Object.h"
#include "Cube.h"
#include "Light.h"

void error_callback(int /* error */, const char *message) {
    cerr << "GLFW error: " << message << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
}

struct AppState {
    Camera camera;
    bool keys[1024];
    bool cameraActive;
};

void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods) {
    auto& as = *static_cast<AppState *>(glfwGetWindowUserPointer(w));

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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,   3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,   3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,          GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,   GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE,               GL_FALSE);
    glfwWindowHint(GLFW_FOCUSED,                 GL_TRUE);

    auto window = glfwCreateWindow(800, 600, "", nullptr, nullptr);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, pos_callback);
    glfwSetMouseButtonCallback(window, ImGui_ImplGlfwGL3_MouseButtonCallback);
    glfwSetScrollCallback(window, ImGui_ImplGlfwGL3_ScrollCallback);
    glfwSetCharCallback(window, ImGui_ImplGlfwGL3_CharCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* Initialize AppState */
    AppState as {
            .camera = { vec3(0.0f, 3.0f, 3.0f), vec3(0.0f, 0.0f, 0.0f) },
            .keys   = { GL_FALSE },
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

    /* Load shaders */
    Program simple { "Simple", "shd/simple.vert", "shd/simple.frag" };
    Program phong  { "Phong",  "shd/phong.vert",  "shd/phong.frag"  };
    Program lamp   { "Light",  "shd/light.vert",  "shd/light.frag"  };

    /* Load data */
    auto cube = Cube();
    cube.model = scale(vec3(2.0f));

    auto light = Light(vec3(1.0f, 1.0f, 1.0f));
    light.model = translate(vec3(5.0f, 0.0f, 0.0f));

    /* Matrices */
    mat4 projection =  perspective(radians(65.0f), 4.0f/ 3.0f, 0.01f, 100.0f);

    auto p_loc  = glGetUniformLocation(phong, "projection");
    auto v_loc  = glGetUniformLocation(phong, "view");
    auto oc_loc = glGetUniformLocation(phong, "objectColor");
    auto lc_loc = glGetUniformLocation(phong, "lightColor");
    auto lp_loc = glGetUniformLocation(phong, "lightPosition");
    auto cp_loc = glGetUniformLocation(phong, "cameraPosition");

    vec3 pos = vec3(4.0f, 0.0f, 0.0f);

    while (!glfwWindowShouldClose(window)) {
        /* Handle input */
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        GLfloat cameraSpeed = 0.1f;

        if (as.cameraActive) {
            if (as.keys[GLFW_KEY_W]) {
                as.camera.position += cameraSpeed * as.camera.front();
            }
            if (as.keys[GLFW_KEY_S]) {
                as.camera.position -= cameraSpeed * as.camera.front();
            }
            if (as.keys[GLFW_KEY_A]) {
                as.camera.position -= as.camera.right() * cameraSpeed;
            }
            if (as.keys[GLFW_KEY_D]) {
                as.camera.position += as.camera.right() * cameraSpeed;
            }
            if (as.keys[GLFW_KEY_LEFT_SHIFT]) {
                as.camera.position -= as.camera.up * cameraSpeed;
            }
            if (as.keys[GLFW_KEY_SPACE]) {
                as.camera.position += as.camera.up * cameraSpeed;
            }
        }

        /* Manipulation */
        ImGui::SliderFloat3("Light position", value_ptr(pos), -10.0f, 10.0f);

        light.model = translate(pos);

        /* Render */
        glClearColor(0.059f, 0.057f, 0.073f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(phong);
            glUniformMatrix4fv(p_loc, 1, GL_FALSE, value_ptr(projection));
            glUniformMatrix4fv(v_loc, 1, GL_FALSE, value_ptr(as.camera.viewMatrix()));

            glUniform3fv(oc_loc, 1, value_ptr(cube.color));
            glUniform3fv(lc_loc, 1, value_ptr(light.color));
            glUniform3fv(lp_loc, 1, value_ptr(vec3(light.model * vec4(0.0f, 0.0f, 0.0f, 1.0f))));
            glUniform3fv(cp_loc, 1, value_ptr(as.camera.position));

            cube.draw(phong);
        glUseProgram(0);

        glUseProgram(lamp);
            glUniformMatrix4fv(glGetUniformLocation(lamp, "projection"), 1, GL_FALSE, value_ptr(projection));
            glUniformMatrix4fv(glGetUniformLocation(lamp, "view"), 1, GL_FALSE, value_ptr(as.camera.viewMatrix()));

            glUniform3fv(glGetUniformLocation(lamp, "lightColor"), 1, value_ptr(light.color));

            light.draw(lamp);
        glUseProgram(0);

        ImGui::Render();

        /* Present */
        glfwSwapBuffers(window);
    }

    /* Cleanup and exit */
    glfwTerminate();
    ImGui_ImplGlfwGL3_Shutdown();

    return 0;
}