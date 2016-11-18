#include "Cube.h"

namespace {
    /* y                z    */
    /* |               /     */
    /*                       */
    /*    3---------7        */
    /*   /|        /|        */
    /*  / |       / |        */
    /* 2---------6  |        */
    /* |  |      |  |        */
    /* |  1------|--5        */
    /* | /       | /         */
    /* |/        |/          */
    /* 0---------4     -- x  */

    static vector<GLfloat> vertices {
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
    };

    static vector<GLuint> indices {
        0, 1, 2,  1, 3, 2,
        0, 2, 4,  4, 2, 6,
        0, 4, 1,  5, 1, 4,
        5, 4, 6,  5, 6, 7,
        6, 2, 3,  6, 3, 7,
        1, 5, 3,  5, 7, 3,
    };
}

Cube::Cube()
    : Object(vertices, indices)
    , color { 0.133f, 0.545f, 0.133f }
{ }