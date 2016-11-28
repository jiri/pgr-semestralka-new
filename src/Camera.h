#pragma once

#include <array>
using namespace std;

#include <GL/glew.h>

#include <glm/glm.hpp>
using namespace glm;

class Camera {
public:
    GLfloat yaw;
    GLfloat pitch;

    vec3 position;
    vec3 up;

    Camera(const vec3 &position, const vec3 &target);

    vec3 front() const;
    vec3 right() const;
    mat4 viewMatrix() const;

    void handleInput(const array<bool, 1024> &keys);

private:
    float speed = 0.1f;
};