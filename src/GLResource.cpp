#include <fmt/format.h>
#include "GLResource.h"

using namespace std;

GLResource::GLResource(function<void(GLsizei, GLuint *)> c, function<void(GLsizei, GLuint *)> d)
    : id {
        [=] { GLuint id = 0; c(1, &id); return id; }()
    }
    , destroy {
        [=] { d(1, &id); }
    }
{ }

GLResource::GLResource(std::function<GLuint()> c, std::function<void(GLuint)> d)
    : id {
        c()
    }
    , destroy {
        [=] { d(id); }
    }
{ }

GLResource::GLResource(GLResource &&other)
    : id { 0 }
{
    swap(*this, other);
}

GLResource &GLResource::operator=(GLResource other) {
    swap(*this, other);

    return *this;
}

GLResource::~GLResource() {
    destroy();
}

void GLResource::swap(GLResource &a, GLResource &b) {
    std::swap(a.id, b.id);
    std::swap(a.destroy, b.destroy);
}
