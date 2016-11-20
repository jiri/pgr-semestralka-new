#pragma once

#include <glm/glm.hpp>
using namespace glm;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
