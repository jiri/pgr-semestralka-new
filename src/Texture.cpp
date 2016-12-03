#include "Texture.h"

#include <SOIL.h>

#include "Program.h"

namespace {
    GLuint glGenTexture() {
        GLuint id;
        glGenTextures(1, &id);
        return id;
    }
}

Texture::Texture()
    : Texture("res/default.png")
{ }

Texture::Texture(const string &path, GLint u, GLenum interpolation)
    : id { glGenTexture() }
    , unit { u }
{
    unique_ptr<unsigned char, decltype(&SOIL_free_image_data)> data {
            SOIL_load_image(path.c_str(), &w, &h, 0, SOIL_LOAD_RGB),
            SOIL_free_image_data,
    };

    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolation);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data.get());
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(Texture &&other) {
    swap(id, other.id);
    swap(unit, other.unit);
}

Texture& Texture::operator=(Texture &&other) {
    using std::swap;

    swap(id, other.id);
    swap(unit, other.unit);

    return *this;
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

template <>
void ProgramHandle::setUniform(string name, const Texture &t) {
    glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(t.unit));
    glBindTexture(GL_TEXTURE_2D, t);

    glUniform1i(location(name), t.unit);
}
