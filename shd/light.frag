#version 330 core

uniform mat4 view;

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};

uniform Light light;

uniform vec3 cameraPosition;

in vec3 vPosition;
in vec3 vNormal;

out vec4 color;

void main() {
    float diff = max(dot(vNormal, normalize(cameraPosition - light.position)), 0.0);
	color = vec4(diff * light.specular, 1.0);
}
