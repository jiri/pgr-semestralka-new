#pragma once

#include <GL/glew.h>

class GLObject {
private:
    static void swap(GLObject &a, GLObject &b);

protected:
    GLuint id;

    GLObject(GLuint id);

public:
    GLObject(const GLObject &other) = delete;
    GLObject& operator=(const GLObject &other) = delete;

    GLObject(GLObject &&other);
    GLObject& operator=(GLObject &&other);

    operator GLuint() const {
        return id;
    }
};
