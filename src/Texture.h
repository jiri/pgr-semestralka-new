#pragma once

#include <iostream>
#include <string>
using namespace std;

#include <GL/glew.h>

#include "GLObject.h"

class Texture {
private:
    GLuint id;
    int w, h;

public:
    Texture();
    Texture(const string &path, GLint unit = 0, GLenum interpolation = GL_NEAREST);

    Texture(const Texture &other) = delete;
    Texture& operator=(const Texture &other) = delete;

    Texture(Texture &&other);
    Texture& operator=(Texture &&other);

    ~Texture();

    operator GLuint() const {
        return id;
    }

    GLint unit;
};