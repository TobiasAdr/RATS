
void main() {

    float distance = distance(pixelPos, torchPos);
    float torchRadius = 300.f;
    vec3 torchColor = vec3(1.0, 0.8, 0.5);
    vec3 torchEffect = torchColor * (1.0 - smoothstep(0.0, torchRadius, distance));   

}