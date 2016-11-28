#include "Camera.h"

#include <GLFW/glfw3.h>

#include <glm/gtx/transform.hpp>

Camera::Camera(const vec3 &p, const vec3 &t)
    : position { p }
    , up { 0.0f, 1.0f, 0.0f }
    , yaw { -90.0f }
    , pitch { -45.0f }
{ }

vec3 Camera::front() const {
    vec3 v;

    v.x = cos(radians(pitch)) * cos(radians(yaw));
    v.y = sin(radians(pitch));
    v.z = cos(radians(pitch)) * sin(radians(yaw));

    return normalize(v);
}

vec3 Camera::right() const {
    return normalize(cross(front(), up));
}

mat4 Camera::viewMatrix() const {
    return lookAt(position, position + front(), up);
}

void Camera::handleInput(const array<bool, 1024> &keys) {
    if (keys[GLFW_KEY_W])          { position += speed * front(); }
    if (keys[GLFW_KEY_S])          { position -= speed * front(); }
    if (keys[GLFW_KEY_A])          { position -= speed * right(); }
    if (keys[GLFW_KEY_D])          { position += speed * right(); }
    if (keys[GLFW_KEY_LEFT_SHIFT]) { position -= speed * up;      }
    if (keys[GLFW_KEY_SPACE])      { position += speed * up;      }
}
