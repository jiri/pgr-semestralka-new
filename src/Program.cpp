#include "Program.h"

#include <iostream>
#include <stdexcept>
using namespace std;

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

namespace {
    GLenum guessTypeFromPath(string path) {
        auto ex = extension(path);

        if (ex == ".vert" || ex == ".vsh") { return GL_VERTEX_SHADER;   }
        if (ex == ".frag" || ex == ".fsh") { return GL_FRAGMENT_SHADER; }

        throw invalid_argument {
            "Unrecognized shader file extension '" + ex + "'"
        };
    }

    char* slurp_file(const char *path) {
        char *buffer = nullptr;
        uint32_t length;

        FILE *f = fopen(path, "rb");
        if (f) {
            /* Get file length */
            fseek(f, 0, SEEK_END);
            length = ftell(f);
            fseek(f, 0, SEEK_SET);

            /* Read data */
            buffer = (char *) malloc(length + 1);
            buffer[length] = 0;
            if (buffer) {
                fread (buffer, 1, length, f);
            }
            fclose (f);
        }

        return buffer;
    }
}

/* Shader implementation */
Shader::Shader(Shader &&other) {
    id = other.id;
    other.id = 0;
}
Shader& Shader::operator=(Shader &&other) {
    id = other.id;
    other.id = 0;

    return *this;
}

Shader::Shader(const char *path)
    : Shader { path, guessTypeFromPath(path) }
{ }

Shader::Shader(string path, GLenum type)
    : id { glCreateShader(type) }
{
    const char *src = slurp_file(path.c_str());
    glShaderSource(id, 1, &src, nullptr);

    glCompileShader(id);

    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(id, 512, nullptr, infoLog);

        throw compilation_error {
            path, infoLog
        };
    }
}

Shader::~Shader() {
    glDeleteShader(id);
}

/* Program implementation */
Program::Program(Program &&other) {
    id = other.id;
    other.id = 0;
}
Program& Program::operator=(Program &&other) {
    glDeleteProgram(id);

    id = other.id;
    other.id = 0;

    return *this;
}

Program::Program(string name, const Shader &vsh, const Shader &fsh)
    : id { glCreateProgram() }
    , name { name }
{
    glAttachShader(id, vsh);
    glAttachShader(id, fsh);

    glLinkProgram(id);

    glDetachShader(id, vsh);
    glDetachShader(id, fsh);

    GLint success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);

    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(id, 512, nullptr, infoLog);

        throw link_error {
                name, infoLog
        };
    }
}

Program::~Program() {
    glDeleteProgram(id);
}