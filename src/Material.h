#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include "Texture.h"

struct Material {
    Texture diffuse;
    Texture specular;
    float shininess;
};