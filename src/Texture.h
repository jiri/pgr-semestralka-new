#pragma once

#include <iostream>
#include <string>
using namespace std;

#include <GL/glew.h>

#include "Program.h"

class Texture {
private:
    GLuint id;

    int w, h;

public:
    Texture(const string &path, GLenum interpolation = GL_NEAREST);
    ~Texture();

    operator GLuint() {
        return id;
    }
};