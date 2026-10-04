#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;

uniform mat4 uViewProj;
uniform vec2 uOffset;
uniform vec2 uScale;
uniform vec2 uUVOffset;
uniform vec2 uUVSize;

out vec2 vUV;

out vec2 vWorldPos;

void main() {
    vec2 worldPos = aPos * uScale + uOffset;
    gl_Position = uViewProj * vec4(worldPos, 0.0, 1.0);
    vUV = uUVOffset + aUV * uUVSize;
    vWorldPos = worldPos;
}