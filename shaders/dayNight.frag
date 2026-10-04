uniform sampler2D texture;
uniform float time;

uniform vec2 torchPos;

void main() {

    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    vec2 pixelPos = gl_FragCoord.xy;

    float daylight = (sin(time) + 1.0) / 2.0; 
    vec3 dayColor  = vec3(1.0, 0.95, 0.8);
    vec3 nightColor = vec3(0.1, 0.15, 0.4);
    vec3 tint = mix(nightColor, dayColor, daylight);
    
    gl_FragColor = vec4(pixel.rgb * tint, pixel.a);
}