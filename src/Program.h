#pragma once

#include <stdexcept>
#include <string>
using namespace std;

#include <GL/glew.h>

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

class Shader {
private:
    GLuint id;

public:
    Shader(const Shader &other) = delete;
    Shader& operator=(const Shader &other) = delete;

    Shader(Shader &&other);
    Shader& operator=(Shader &&other);

    Shader(const char *path);
    Shader(string path, GLenum type);
    ~Shader();

    operator GLuint() const {
        return id;
    }
};

class Program {
private:
    GLuint id;

public:
    const string name;

    Program(const Program &other) = delete;
    Program& operator=(const Program &other) = delete;

    Program(Program &&other);
    Program& operator=(Program &&other);

    Program(string name, const Shader &vsh, const Shader &fsh);
    ~Program();

    operator GLuint() const {
        return id;
    }
};