#pragma once
#include "OpenGLElementSimulation.h"

class OpenGLFireSimulation : public OpenGLElementSimulation
{
public:
    OpenGLFireSimulation(std::vector<sf::CircleShape>& colliders, int width, int height); 

    void init() override;
    void update(float deltaTime, int enemyCount);
    void spawnFire(int posX, int posY, float dirX, float dirY);

    GLuint getTexture() const override { return this->fireTexture; }

private:
    GLuint fireTexture;
    GLuint fireParticleSSBO;

    GLuint colorProgram;
    GLuint explodeProgram;

    static const int POOL_SIZE = 6;
    int spawnOffset = 0;

    FireParticleCPU particles[POOL_SIZE];

    void stepFire(float deltaTime);
    void colorFire();
    void explode(float deltaTime, int enemyCount);
    void spawnExplosionGPU(int slot, float posX, float posY); 
};