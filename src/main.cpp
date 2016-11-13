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
#include "SceneGraph.h"

void error_callback(int /* error */, const char *message) {
    cerr << "GLFW error: " << message << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
}

struct AppState {
    Camera camera;
    bool keys[1024];
};

void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods) {
    auto& as = *static_cast<AppState *>(glfwGetWindowUserPointer(w));

    if (action == GLFW_PRESS) {
        as.keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        as.keys[key] = false;
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

    as.camera.yaw   += dx * 0.1f;
    as.camera.pitch -= dy * 0.1f;

    if (as.camera.pitch > 89.0f) {
        as.camera.pitch = 89.0f;
    }
    if (as.camera.pitch < -89.0f) {
        as.camera.pitch = -89.0f;
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* Initialize AppState */
    AppState as {
            .camera = { vec3(0.0f, 3.0f, 3.0f), vec3(0.0f, 0.0f, 0.0f) },
            .keys   = { GL_FALSE },
    };

    glfwSetWindowUserPointer(window, &as);

    /* Initialize ImGui */
    // ImGui_ImplGlfwGL3_Init(window, true);

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

    /* Load data */
    SceneGraph sceneGraph { move(make_unique<Cube>()) };
//    Cube cube;

    /* Matrices */
    mat4 mvp;

    mvp *= perspective(radians(65.0f), 4.0f/ 3.0f, 0.01f, 100.0f);
    // mvp *= lookAt(vec3(0.0f, 3.0f, 3.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));

    GLint mvp_loc = glGetUniformLocation(simple, "MVP");

    while (!glfwWindowShouldClose(window)) {
        /* Handle input */
        glfwPollEvents();

        GLfloat cameraSpeed = 0.1f;

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

        /* Render */
        glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 mvp_ { mvp };

        mvp_ *= as.camera.viewMatrix();
        // mvp_ *= rotate((GLfloat) glfwGetTime(), vec3(0.3f, 0.8f, 1.0f));

        glUseProgram(simple);
            glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, value_ptr(mvp_));

            sceneGraph.draw();
        glUseProgram(0);

        /* Present */
        glfwSwapBuffers(window);
    }

    /* Cleanup and exit */
    glfwTerminate();
    return 0;
}