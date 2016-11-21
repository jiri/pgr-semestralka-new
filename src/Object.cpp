#include "Object.h"

#include <iostream>
using namespace std;

#include <imgui.h>
#include <tiny_obj_loader.h>
#include <GLFW/glfw3.h>

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

    template <typename T>
    GLvoid * offset(GLint i) {
        return (GLvoid *) (i * sizeof(T));
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
    , visible { GL_TRUE }
    , model { 1.0f }
{
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), offset<GLfloat>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), offset<GLfloat>(3));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

Object::Object(const vector<GLfloat> &vertices, const vector<GLuint> &indices)
    : Object()
{
    loadData(vertices, indices);
}

Object::Object(const string &filename)
    : Object()
{
    tinyobj::attrib_t attrib;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str());

    if (!err.empty()) {
        cerr << err << endl;
    }

    if (!ret) {
        throw "Failed to open file";
    }

    vector<GLfloat> vertices;
    vector<GLuint> indices;

    for (const auto &shape : shapes) {
        for (const auto &index : shape.mesh.indices) {
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            vertices.push_back(attrib.normals[3 * index.normal_index + 0]);
            vertices.push_back(attrib.normals[3 * index.normal_index + 1]);
            vertices.push_back(attrib.normals[3 * index.normal_index + 2]);

            indices.push_back(indices.size());
        }
    }

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

void Object::draw(Program &p) const {
    if (visible) {
        glBindVertexArray(vao);

        auto loc = glGetUniformLocation(p, "model");
        glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(model));

        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);
    }

//    for (auto &child : children) {
//        child->draw(p);
//    }
}

//void Object::edit() {
//    ImGui::Checkbox("visible", &visible);
//}