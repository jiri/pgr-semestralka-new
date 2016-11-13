#pragma once

#include <vector>
using namespace std;

#include <GL/glew.h>

#include "SceneGraph.h"

class Object : public SceneNode {
private:
    GLuint vao;
    GLuint vbo, ebo;

    GLuint numIndices;

public:
    Object(const Object &other) = delete;
    Object& operator=(const Object &other) = delete;

    Object(Object &&other);
    Object& operator=(Object &&other);

    Object();
    Object(const vector<GLfloat> &vertices, const vector<GLuint> &indices);
    virtual ~Object();

    void loadData(const vector<GLfloat> &vertices, const vector<GLuint> &indices);
    void draw() const;
};
