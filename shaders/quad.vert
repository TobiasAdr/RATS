#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;

uniform vec2 uOffset;
uniform vec2 uScale;
uniform mat4 uViewProj;
uniform int uUseWorldUV;

out vec2 vWorldPos;
out vec2 vUV;

void main() {
    vec2 worldPos = aPos * uScale + uOffset;
    vWorldPos = worldPos;
    vUV = aUV;
    gl_Position = uViewProj * vec4(worldPos, 0.0, 1.0);
}