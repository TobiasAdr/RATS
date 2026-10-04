#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in float aT;

uniform mat4 uViewProj;
out float vT;

void main() {
    vT = aT;
    gl_Position = uViewProj * vec4(aPos, 0.0, 1.0);
}