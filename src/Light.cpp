//
// Created by Jiří Šebele on 18/11/2016.
//

#include "Light.h"

#include "Program.h"

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

Light::Light(const vec3 &c)
    : Object(vertices, indices)
    , ambient  { 0.2f * c }
    , diffuse  { 0.5f * c }
    , specular { 1.0f * c }
{ }

template <>
void Program::setUniform(string name, const Light &l) {
    setUniform(name + ".ambient",  l.ambient);
    setUniform(name + ".diffuse",  l.diffuse);
    setUniform(name + ".specular", l.specular);
}