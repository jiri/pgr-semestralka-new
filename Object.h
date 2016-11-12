#pragma once

#include <vector>
using namespace std;

#include <GL/glew.h>

class Object {
private:
    GLuint vao;
    GLuint vbo, ebo;

    GLuint numIndices;

public:
    Object(const Object &other) = delete;
    Object& operator=(const Object &other) = delete;

    Object(Object &&other);
    Object& operator=(Object &&other);

    Object(const vector<GLfloat> vertices, const vector<GLuint> indices);
    virtual ~Object();

    void draw() const;
};
