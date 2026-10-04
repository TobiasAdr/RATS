#version 430 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec2 aInstancePos;
layout(location = 3) in float aHit;

out vec2 vUV;
out vec2 vWorldPos; 
out float vHit;

uniform mat4 uProjection;

void main() {

    vHit = aHit;
    vec2 worldPos = aPos + aInstancePos;
    gl_Position = uProjection * vec4(worldPos, 0.0, 1.0);
    vUV = aUV;
    vWorldPos = worldPos;

}