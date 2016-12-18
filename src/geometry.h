#pragma once

#include <vector>

#include <GL/glew.h>

class geometry {
public:
    geometry(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices);
    geometry(const geometry &other) = delete;
    geometry(geometry &&other);

    geometry& operator=(geometry other);

    ~geometry();

    void draw() const;

private:
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;

    GLuint numIndices = 0;

    static void swap(geometry &a, geometry &b);
};
