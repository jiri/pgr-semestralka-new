#include <iostream>
#include <vector>
using namespace std;

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include "Program.h"
#include "Object.h"

void error_callback(int /* error */, const char *message) {
    cerr << "GLFW error: " << message << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
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

    /* Initialize ImGui */
    ImGui_ImplGlfwGL3_Init(window, true);

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

    /* Create data */
                               /*                       */
                               /* y                z    */
    vector<GLfloat> vertices { /* |               /     */
      -0.5f, -0.5f, -0.5f,     /*                       */
      -0.5f, -0.5f,  0.5f,     /*    3---------7        */
      -0.5f,  0.5f, -0.5f,     /*   /|        /|        */
      -0.5f,  0.5f,  0.5f,     /*  / |       / |        */
       0.5f, -0.5f, -0.5f,     /* 2---------6  |        */
       0.5f, -0.5f,  0.5f,     /* |  |      |  |        */
       0.5f,  0.5f, -0.5f,     /* |  1------|--5        */
       0.5f,  0.5f,  0.5f,     /* | /       | /         */
    };                         /* |/        |/          */
                               /* 0---------4     -- x  */
                               /*                       */

    vector<GLuint> indices {
      0, 1, 2,  1, 3, 2,
      0, 2, 4,  4, 2, 6,
      0, 4, 1,  5, 1, 4,
      5, 4, 6,  5, 6, 7,
      6, 2, 3,  6, 3, 7,
      1, 5, 3,  5, 7, 3,
    };

    Object cube { vertices, indices };

    while (!glfwWindowShouldClose(window)) {
        /* Handle input */
        glfwPollEvents();

        /* Render */
        glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(simple);
            cube.draw();
        glUseProgram(0);

        /* Present */
        glfwSwapBuffers(window);
    }

    /* Cleanup and exit */
    glfwTerminate();
    return 0;
}