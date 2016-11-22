#include "Texture.h"

#include <SOIL.h>

Texture::Texture(const string &path) {
    unique_ptr<unsigned char, decltype(&SOIL_free_image_data)> data {
            SOIL_load_image(path.c_str(), &w, &h, 0, SOIL_LOAD_RGB),
            SOIL_free_image_data,
    };

    glGenTextures(1, &id);

    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data.get());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

template <>
void Program::setUniform(string name, const Texture &t) {
    glBindTexture(GL_TEXTURE_2D, id);
    glUniform1i(location(name), 0);
}