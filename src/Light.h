#pragma once

#include "Object.h"

#include <glm/glm.hpp>
using namespace glm;

class Light : public Object {
public:
    Light(const vec3 &color);

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
