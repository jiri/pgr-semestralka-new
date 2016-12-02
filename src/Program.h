#pragma once

#include <string>
#include <typeinfo>
using namespace std;

#include <boost/filesystem.hpp>

#include <GL/glew.h>
#include <boost/filesystem/path.hpp>

#include "GLObject.h"

class Shader : public GLObject {
public:
    Shader(boost::filesystem::path p);
    Shader(boost::filesystem::path p, GLenum type);
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
    Program(boost::filesystem::path vpath, boost::filesystem::path fpath);
    Program(const Shader &vsh, const Shader &fsh);
    ~Program();

    GLint location(string name) const;

    template<typename T>
    void setUniform(string name, const T &val);

    ProgramHandle use() {
        return { id };
    }
};
