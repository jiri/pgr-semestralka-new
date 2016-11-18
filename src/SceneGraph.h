#pragma once

#include <vector>
using namespace std;

#include <glm/glm.hpp>
using namespace glm;

#include <imgui.h>

#include "Program.h"

/* TODO: Node<T> = SceneGraph.add_child(T) */

class SceneNode {
private:
    bool selected;

public:
    vector<unique_ptr<SceneNode>> children;

    mat4 model;

    SceneNode()
        : model { 1.0f }
        , selected { GL_FALSE }
    { }

    void add_child(unique_ptr<SceneNode> &&ptr) {
        children.push_back(move(ptr));
    }

    virtual void draw(Program &program) const = 0;
    virtual void edit() = 0;

    void tree_view() {
        if (children.size() == 0) {
            ImGui::Selectable("node", &selected);
        } else {
            if (ImGui::TreeNode("node")) {
                for (auto &child : children) {
                    child->tree_view();
                }

                ImGui::TreePop();
            }
        }
    }

    vec3 center() {
        return model * vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
};

class SceneGraph {
public:
    unique_ptr<SceneNode> root;

    SceneGraph(unique_ptr<SceneNode> &&root)
        : root { move(root) }
    { }

    void draw(Program &p) {
        root->draw(p);
    }

    void tree_view() {
        ImGui::Begin("SceneGraph");
        root->tree_view();
        ImGui::End();
    }
};