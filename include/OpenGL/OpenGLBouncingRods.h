#pragma once
#include "OpenGLElementSimulation.h"

class OpenGLBouncingRods : public OpenGLElementSimulation
{
public:
    OpenGLBouncingRods(std::vector<sf::CircleShape>& colliders, int width, int height); 

    void init() override;
    void update(float deltaTime, int enemyCount);
    void spawnRods(int posX, int posY, float dirX, float dirY);
    GLuint getTexture() const override { return this->rodTexture; }

private:
    void colorRods();
    void updateRods(float deltaTime, int enemyCount);

    GLuint rodTexture;
    GLuint rodParticleSSBO;

    GLuint rodProgram;
    GLuint colorRodProgram;
    GLuint updateRodProgram;

    int spawnOffset = 0;
    static const int POOL_SIZE = 300;
};