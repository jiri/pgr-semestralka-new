#include <string>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "Program.h"

/* Scalars */
template<>
void Program::setUniform(string name, const GLfloat &f) {
    glUniform1f(location(name), f);
}

template<>
void Program::setUniform(string name, const GLint &i) {
    glUniform1i(location(name), i);
}

/* Vectors */
template<>
void Program::setUniform(string name, const vec2 &v) {
    glUniform2fv(location(name), 1, value_ptr(v));
}

template<>
void Program::setUniform(string name, const vec3 &v) {
    glUniform3fv(location(name), 1, value_ptr(v));
}

template<>
void Program::setUniform(string name, const vec4 &v) {
    glUniform4fv(location(name), 1, value_ptr(v));
}

/* Matrices */
template<>
void Program::setUniform(string name, const mat3 &m) {
    glUniformMatrix3fv(location(name), 1, GL_FALSE, value_ptr(m));
}

template<>
void Program::setUniform(string name, const mat4 &m) {
    glUniformMatrix4fv(location(name), 1, GL_FALSE, value_ptr(m));
}