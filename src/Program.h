#pragma once

#include <stdexcept>
#include <string>
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
    string name;
    string infoLog;

    link_error(const string &name, const string &info)
        : name { name }
        , infoLog { info }
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
public:
    const string name;

    Program(string name, const Shader &vsh, const Shader &fsh);
    ~Program();

    GLint location(string name) const;

    template<typename T>
    void setUniform(string name, const T &val) {
        throw domain_error {
                "Program::setUniform not defined for " + string(typeid(T).name())
        };
    }
};