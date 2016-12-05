#pragma once

#include <string>
#include <gsl.h>

#include <boost/filesystem.hpp>
#include <GL/glew.h>

#include "GLObject.h"

class Shader : public GLObject {
public:
    Shader(gsl::czstring p);
    Shader(boost::filesystem::path p);
    Shader(boost::filesystem::path p, GLenum type);
    ~Shader();
};

class ProgramHandle {
    friend class Program;

public:
    ~ProgramHandle();

    GLint location(std::string name) const;

    template<typename T>
    void setUniform(std::string name, const T &val);

protected:
    ProgramHandle(GLuint id);

private:
    GLuint program;
};

class Program : public GLObject {
public:
    Program(const Shader &vsh, const Shader &fsh);
    ~Program();

    ProgramHandle use();
};
