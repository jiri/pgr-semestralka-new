#pragma once

#include <string>
#include <typeinfo>
using namespace std;

#include <GL/glew.h>

#include "GLObject.h"

class Shader : public GLObject {
public:
    Shader(const char *path);
    Shader(string path, GLenum type);
    ~Shader();
};

class Program : public GLObject {
private:
    class ProgramHandle {
    public:
        ProgramHandle(GLuint id) {
            glUseProgram(id);
        }

        ~ProgramHandle() {
            glUseProgram(0);
        }
    };

public:
    Program(const Shader &vsh, const Shader &fsh);
    ~Program();

    GLint location(string name) const;

    template<typename T>
    void setUniform(string name, const T &val);

    ProgramHandle use() {
        return { id };
    }
};
