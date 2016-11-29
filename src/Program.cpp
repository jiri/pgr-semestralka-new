#include "Program.h"

#include <iostream>
#include <stdexcept>
#include <fstream>
using namespace std;

#include <boost/filesystem.hpp>

#include <fmt/format.h>

namespace {
    GLenum guessTypeFromPath(string path) {
        auto ex = boost::filesystem::extension(path);

        if (ex == ".vert" || ex == ".vsh") { return GL_VERTEX_SHADER;   }
        if (ex == ".frag" || ex == ".fsh") { return GL_FRAGMENT_SHADER; }

        throw invalid_argument {
            fmt::format("Unrecognized shader file extension '{}'", ex)
        };
    }

    string slurp_file(const string &path) {
        ifstream file { path };

        if (!file.is_open()) {
            throw std::runtime_error {
                fmt::format("Failed to open file '{}'", path)
            };
        }

        return {
            istreambuf_iterator<char> { file },
            istreambuf_iterator<char> {      },
        };
    }
}

/* Shader implementation */
Shader::Shader(const char *path)
    : Shader { path, guessTypeFromPath(path) }
{ }

Shader::Shader(string path, GLenum type)
    : GLObject { glCreateShader(type) }
{
    const auto src = slurp_file(path);
    const auto psrc = src.c_str();

    glShaderSource(id, 1, &psrc, nullptr);

    glCompileShader(id);

    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(id, 512, nullptr, infoLog);

        throw runtime_error {
            fmt::format("Compilation of shader '{}' failed:\n{}", path, infoLog)
        };
    }
}

Shader::~Shader() {
    glDeleteShader(id);
}

/* Program implementation */
Program::Program(const Shader &vsh, const Shader &fsh)
    : GLObject { glCreateProgram() }
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

        throw runtime_error {
            fmt::format("Program linking failed:\n{}", infoLog)
        };
    }
}

Program::~Program() {
    glDeleteProgram(id);
}

GLint Program::location(string name) const {
    return glGetUniformLocation(id, name.c_str());
}
