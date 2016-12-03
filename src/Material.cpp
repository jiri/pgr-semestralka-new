#include "Material.h"

#include "Program.h"

template <>
void ProgramHandle::setUniform(string name, const Material &m) {
    setUniform(name + ".diffuse",   m.diffuse);
    setUniform(name + ".specular",  m.specular);
    setUniform(name + ".shininess", m.shininess);
}
