#version 450
out vec4 FragColor;

void main() {
    vec2 center = gl_PointCoord - vec2(0.5);
    float dist = length(center);
    float glow = 1.0 - smoothstep(0.0, 0.5, dist);

    vec3 color = vec3(1.0, 0.4, 0.1);
    FragColor = vec4(color * glow, glow);
}