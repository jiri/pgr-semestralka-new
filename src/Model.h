#pragma once

#include <vector>
#include <string>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <GL/glew.h>

#include "Program.h"

class Model {
public:
    GLuint vao;
    GLuint vbo, ebo;

    GLuint numIndices;

    bool visible;

    mat4 model;

public:
    Model(const Model &other) = delete;
    Model& operator=(const Model &other) = delete;

    Model(Model &&other);
    Model& operator=(Model &&other);

    Model();
    Model(const vector<GLfloat> &vertices, const vector<GLuint> &indices);
    Model(const string &filename);
    virtual ~Model();

    void loadData(const vector<GLfloat> &vertices, const vector<GLuint> &indices);
    void draw(Program &program) const;
};
