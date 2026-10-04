#version 450

struct FirePlaceParticle {
    float posX, posY;
    float dirX, dirY;
    float life;
};

layout(std430, binding = 0) buffer Particles {
    FirePlaceParticle particles[];
};

uniform mat4 uViewProj;

void main() {
    FirePlaceParticle p = particles[gl_VertexID];
    
    gl_PointSize = 5.0;
    gl_Position = uViewProj * vec4(p.posX, p.posY, 0.0, 1.0);
}

