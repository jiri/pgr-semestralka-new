#pragma once

#include <vector>
#include <string>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <GL/glew.h>

#include "Program.h"
#include "Material.h"

class Model {
protected:
    GLuint vao;
    GLuint vbo, ebo;

    GLuint numIndices;

    void loadData(const vector<GLfloat> &vertices, const vector<GLuint> &indices);

public:
    Model();
    Model(const string &filename);

    Model(const Model &other) = delete;
    Model& operator=(const Model &other) = delete;

    Model(Model &&other);
    Model& operator=(Model &&other);

    virtual ~Model();

    void draw() const;

    Material material;
};
