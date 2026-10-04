#version 430 core

in vec2 vUV;
in vec2 vWorldPos;
in float vHit;
out vec4 FragColor;

uniform sampler2D uTexture;

uniform vec2 uFirePos;
uniform vec3 uAmbient;

void main() {

    vec4 texColor = texture(uTexture, vec2(1.0 - vUV.x, 1.0 - vUV.y));

    float dist = distance(vWorldPos, uFirePos);

    vec3 lightColor = vec3(0.0);


    lightColor += vec3(0.4, 0.15, 0.05) * (1.0 - smoothstep(60.0, 100.0, dist));
    lightColor += vec3(0.9, 0.4, 0.15) * (1.0 - smoothstep(30.0,  60.0, dist));
    lightColor += vec3(1.5, 1.1, 0.5)  * (1.0 - smoothstep(15.0,  30.0, dist));
    lightColor += vec3(2.0, 1.8, 1.4)  * (1.0 - smoothstep(0.0,   15.0, dist));

    vec3 finalColor = texColor.rgb * (uAmbient + lightColor);

    if(vHit == 1){
        
        FragColor = vec4(finalColor * 5, texColor.a);

    }

    else{

        FragColor = vec4(finalColor, texColor.a);
    
    }
}