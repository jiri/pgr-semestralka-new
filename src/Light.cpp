#include "Light.h"

#include "Program.h"

Light::Light(const vec3 &c)
    : Model { "res/bulb2.obj" }
    , ambient  { 0.2f * c }
    , diffuse  { 0.5f * c }
    , specular { 1.0f * c }
{ }

template <>
void ProgramHandle::setUniform(string name, const Light &l) {
    setUniform(name + ".ambient",  l.ambient);
    setUniform(name + ".diffuse",  l.diffuse);
    setUniform(name + ".specular", l.specular);
}
