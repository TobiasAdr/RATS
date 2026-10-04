#version 330 core

in vec2 vWorldPos;
in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec3 uColor;
uniform float uTextureScale;
uniform int uUseWorldUV;
uniform float uTileW;
uniform float uTileH;

uniform vec2 uFirePos;
uniform vec3 uAmbient;

uniform int isWindow;

void main() {

    float uFireStrength = 1.f;
    vec2 uv;
    if (uUseWorldUV == 1) {
        float gridX = vWorldPos.x / (uTileW * 0.5) + vWorldPos.y / (uTileH * 0.5);
        float gridY = -vWorldPos.x / (uTileW * 0.5) + vWorldPos.y / (uTileH * 0.5);
        uv = vec2(gridX, gridY) / uTextureScale;
    } else {
        uv = vUV * uTextureScale;
    }

    vec4 texColor = texture(uTexture, uv);

    float dist = distance(vWorldPos, uFirePos);

    vec3 lightColor = vec3(0.0);
   
    lightColor += vec3(0.4, 0.15, 0.05) * (1.0 - smoothstep(60.0, 100.0, dist)) * uFireStrength;
    lightColor += vec3(0.9, 0.4,  0.15) * (1.0 - smoothstep(30.0,  60.0, dist)) * uFireStrength;
    lightColor += vec3(1.5, 1.1,  0.5)  * (1.0 - smoothstep(15.0,  30.0, dist)) * uFireStrength;
    lightColor += vec3(2.0, 1.8,  1.4)  * (1.0 - smoothstep(0.0,   15.0, dist)) * uFireStrength;

    vec3 baseColor = texColor.rgb * uColor;
    vec3 finalColor = baseColor * (uAmbient + lightColor);


    if(isWindow == 1){
        FragColor = vec4(baseColor, texColor.a);
    }

    else{
        FragColor = vec4(finalColor, texColor.a);
    }
}

