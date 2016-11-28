#pragma once

#include <stdexcept>
#include <string>
#include <typeinfo>
using namespace std;

#include <GL/glew.h>

#include "GLObject.h"

class compilation_error : public exception {
public:
    string path;
    string infoLog;

    compilation_error(const string &path, const string &info)
        : path { path }
        , infoLog { info }
    { }

    const char* what() {
        return "Shader compilation failed";
    }
};

class link_error : public exception {
public:
    string infoLog;

    link_error(const string &info)
        : infoLog { info }
    { }

    const char* what() {
        return "Program linking failed";
    }
};

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
