#pragma once

#include "Object.h"

#include <glm/glm.hpp>
using namespace glm;

class Light : public Object {
public:
    Light(const vec3 &color);

    vec3 color;
};
