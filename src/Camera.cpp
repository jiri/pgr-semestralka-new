#include "Camera.h"

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