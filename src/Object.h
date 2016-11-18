#pragma once

#include <vector>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <GL/glew.h>

#include "Program.h"

class Object {
public:
    GLuint vao;
    GLuint vbo, ebo;

    GLuint numIndices;

    bool visible;

    mat4 model;

public:
    Object(const Object &other) = delete;
    Object& operator=(const Object &other) = delete;

    Object(Object &&other);
    Object& operator=(Object &&other);

    Object();
    Object(const vector<GLfloat> &vertices, const vector<GLuint> &indices);
    virtual ~Object();

    void loadData(const vector<GLfloat> &vertices, const vector<GLuint> &indices);
    void draw(Program &program) const;
    // void edit();
};
