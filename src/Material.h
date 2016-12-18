#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include "Texture.h"
#include "Program.h"

struct Material {
    Texture diffuse;
    Texture specular;
    float shininess;
};

struct material {
    Program program;
};

struct shadeless : public material {
    shadeless(Texture texture)
        : program { "shd/shadeless.vert", "shd/shadeless.frag" }
        , diffuse { std::move(texture) }
    { }

    Texture diffuse;
};

struct blank : public material {
    blank()
        : program { "shd/light.vert", "shd/light.frag" }
    { }
};
