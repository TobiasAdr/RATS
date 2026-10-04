#version 330 core
in float vT;
out vec4 FragColor;

void main() {
    float fade = vT * (1.0 - vT) * 4.0;
    FragColor = vec4(1.0, 0.92, 0.6, fade * 0.04);
}