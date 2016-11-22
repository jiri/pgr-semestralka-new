#version 330 core

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};

uniform Light light;

in vec3 vPosition;

out vec4 color;

void main() {
	color = vec4(light.specular, 1.0) * (1.8 - pow(length(vPosition), 4));
}
