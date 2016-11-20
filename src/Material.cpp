#include "Material.h"

#include "Program.h"

template <>
void Program::setUniform(string name, const Material &m) {
    setUniform(name + ".ambient",   m.ambient);
    setUniform(name + ".diffuse",   m.diffuse);
    setUniform(name + ".specular",  m.specular);
    setUniform(name + ".shininess", m.shininess);
}