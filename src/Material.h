#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include "Texture.h"

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct TexturedMaterial {
    Texture diffuse;
    Texture specular;
    float shininess;
};