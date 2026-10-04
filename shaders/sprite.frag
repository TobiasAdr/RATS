#version 330 core
in vec2 vUV;
in vec2 vWorldPos;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec2 uFirePos;
uniform vec3 uAmbient;

void main() {
    vec4 texColor = texture(uTexture, vUV);
    if (texColor.a < 0.1) discard;

    float uFireStrength = 0.5;
    float dist = distance(vWorldPos, uFirePos);

    vec3 lightColor = vec3(0.0);
    lightColor += vec3(0.4, 0.15, 0.05) * (1.0 - smoothstep(60.0, 100.0, dist)) * uFireStrength;
    lightColor += vec3(0.9, 0.4,  0.15) * (1.0 - smoothstep(30.0,  60.0, dist)) * uFireStrength;
    lightColor += vec3(1.5, 1.1,  0.5)  * (1.0 - smoothstep(15.0,  30.0, dist)) * uFireStrength;
    lightColor += vec3(2.0, 1.8,  1.4)  * (1.0 - smoothstep(0.0,   15.0, dist)) * uFireStrength;

    vec3 finalColor = texColor.rgb * (uAmbient + lightColor);

    FragColor = vec4(finalColor, texColor.a);
}