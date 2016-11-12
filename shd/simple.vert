#version 330 core

uniform mat4 MVP;

layout (location = 0) in vec3 position;

out vec4 vPos;

void main() {
	gl_Position = vPos = MVP * vec4(position, 1.0);
}
