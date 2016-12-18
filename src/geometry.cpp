#include "geometry.h"

using namespace std;

namespace {
    template <typename T>
    GLvoid* offset(GLint i) {
        return static_cast<GLvoid*>(i * sizeof(T));
    }
}

geometry::geometry(const vector<GLfloat> &vs, const vector<GLuint> &is) {
    numIndices = static_cast<GLuint>(is.size());

    glGenVertexArrays(1, &m_vao);

    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    glBufferData(GL_ARRAY_BUFFER,         vs.size() * sizeof(GLfloat), vs.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, is.size() * sizeof(GLfloat), is.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), offset<GLfloat>(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), offset<GLfloat>(3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), offset<GLfloat>(6));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

geometry::geometry(geometry &&other) {
    swap(*this, other);
}

geometry &geometry::operator=(geometry other) {
    swap(*this, other);
    return *this;
}

geometry::~geometry() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void geometry::draw() const {
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void geometry::swap(geometry &a, geometry &b) {
    std::swap(a.m_vao, b.m_vao);
    std::swap(a.m_vbo, b.m_vbo);
    std::swap(a.m_ebo, b.m_ebo);
}
