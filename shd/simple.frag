#version 330 core

in vec4 vPos;

out vec4 color;

void main() {
//    color = vec4(0.898, 0.867, 0.796, 1.0);
    color = vec4(vPos.zzz * vPos.zzz / 32.0, 1.0);
}
