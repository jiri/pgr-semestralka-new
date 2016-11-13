#pragma once

#include <glm/glm.hpp>
using namespace glm;

class SceneNode {
public:
    mat4 transformation;


};

class ObjectNode : public SceneNode {
private:
};

class TransformationNode : public SceneNode {
};

class SceneGraph {
public:
    SceneNode root;
};