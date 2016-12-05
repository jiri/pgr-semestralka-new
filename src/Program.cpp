#include "Program.h"

#include <stdexcept>
using namespace std;

using namespace boost::filesystem;

#include <fmt/format.h>

namespace {
    GLenum guessTypeFromPath(path p) {
        auto ex = p.extension().string();

        if (ex == ".vert" || ex == ".vsh") { return GL_VERTEX_SHADER;   }
        if (ex == ".frag" || ex == ".fsh") { return GL_FRAGMENT_SHADER; }

        throw invalid_argument {
            fmt::format("Unrecognized shader file extension '{}'", ex)
        };
    }

    string slurp_file(path p) {
        boost::filesystem::ifstream file { p };

        if (!file.is_open()) {
            throw runtime_error {
                fmt::format("Failed to open file '{}'", p.string())
            };
        }

        return {
            istreambuf_iterator<char> { file },
            istreambuf_iterator<char> {      },
        };
    }
}

/* Shader implementation */
Shader::Shader(gsl::czstring p)
    : Shader { path { p } }
{ }

Shader::Shader(path p)
    : Shader { p, guessTypeFromPath(p) }
{ }

Shader::Shader(path p, GLenum type)
    : GLObject { glCreateShader(type) }
{
    const auto src = slurp_file(p);
    const auto psrc = src.c_str();

    glShaderSource(id, 1, &psrc, nullptr);

    glCompileShader(id);

    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(id, 512, nullptr, infoLog);

        throw runtime_error {
            fmt::format("Compilation of shader '{}' failed:\n{}", p.string(), infoLog)
        };
    }
}

Shader::~Shader() {
    glDeleteShader(id);
}

/* ProgramHandle implementation */
ProgramHandle::ProgramHandle(GLuint id)
    : program { id }
{
    glUseProgram(id);
}

ProgramHandle::~ProgramHandle() {
    glUseProgram(0);
}

GLint ProgramHandle::location(string name) const {
    return glGetUniformLocation(program, name.c_str());
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

ProgramHandle Program::use() {
    return { id };
}
