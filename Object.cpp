#include "Object.h"

namespace {
    GLuint glGenVertexArray() {
        GLuint id;
        glGenVertexArrays(1, &id);
        return id;
    }

    GLuint glGenBuffer() {
        GLuint id;
        glGenBuffers(1, &id);
        return id;
    }

    void glDeleteVertexArray(GLuint id) {
        glDeleteVertexArrays(1, &id);
    }

    void glDeleteBuffer(GLuint id) {
        glDeleteBuffers(1, &id);
    }
}

Object::Object(Object &&other) {
    vao = other.vao;
    vbo = other.vbo;
    ebo = other.ebo;

    other.vao = 0;
    other.vbo = 0;
    other.ebo = 0;
}

Object& Object::operator=(Object &&other) {
    glDeleteVertexArray(vao);
    glDeleteBuffer(vbo);
    glDeleteBuffer(ebo);

    vao = other.vao;
    vbo = other.vbo;
    ebo = other.ebo;

    other.vao = 0;
    other.vbo = 0;
    other.ebo = 0;

    return *this;
}

Object::Object()
    : vao { glGenVertexArray() }
    , vbo { glGenBuffer() }
    , ebo { glGenBuffer() }
    , numIndices { 0 }
{
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Object::Object(const vector<GLfloat> &vertices, const vector<GLuint> &indices)
    : Object()
{
    loadData(vertices, indices);
}

Object::~Object() {
    glDeleteVertexArray(vao);
    glDeleteBuffer(vbo);
    glDeleteBuffer(ebo);
}

void Object::loadData(const vector<GLfloat> &vertices, const vector<GLuint> &indices) {
    numIndices = static_cast<GLuint>(indices.size());

    glBindVertexArray(vao);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLfloat), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Object::draw() const {
    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}