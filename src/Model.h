#pragma once

#include <GL/glew.h>

#include "Material.h"
#include "geometry.h"

class model {
public:
    model(boost::filesystem::path p);

private:
    model::model(tuple<vector<GLfloat>, vector<GLuint>, material> t);

    geometry g;
    material m;
};
