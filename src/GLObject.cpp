//
// Created by Jiří Šebele on 19/11/2016.
//

#include "GLObject.h"

#include <utility>

void GLObject::swap(GLObject &a, GLObject &b) {
    using std::swap;

    swap(a.id, b.id);
}

GLObject::GLObject(GLuint id)
    : id { id }
{ }

GLObject::GLObject(GLObject &&other)
    : id { 0 }
{
    swap(*this, other);
}

GLObject& GLObject::operator=(GLObject &&other) {
    swap(*this, other);
    return *this;
}
