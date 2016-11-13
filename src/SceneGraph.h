#pragma once

#include <vector>
using namespace std;

#include <glm/glm.hpp>
using namespace glm;

class SceneNode {
public:
    vector<unique_ptr<SceneNode>> children;

    void add_child(unique_ptr<SceneNode> &&ptr) {
        children.push_back(move(ptr));
    }

    virtual void draw() const = 0;
};

class SceneGraph {
public:
    unique_ptr<SceneNode> root;

    SceneGraph(unique_ptr<SceneNode> &&root)
        : root { move(root) }
    { }

    void draw() {
        root->draw();
    }
};