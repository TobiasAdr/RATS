#pragma once
#include "OpenGLElementSimulation.h"
#include <cmath>
#include <numbers>
#include <fstream>
#include <string>

class OpenGLWaterSimulation : public OpenGLElementSimulation
{
public:
    OpenGLWaterSimulation(std::vector<sf::CircleShape>& colliders, int width, int height); 

    void update(float deltaTime, int enemyCount);
    void spawnWaterComputeShader(float x, float y, float dirX, float dirY);

    GLuint getTexture() const override { return waterTexture; }

private:
    GLuint spawnProgram;
    GLuint stepProgram;
    GLuint waterRenderCompProgram;
    GLuint clearCompProgram;
    GLuint particleSSBO;
    GLuint waterTexture;

    static const int PARTICLES_PER_SPAWN = 64;
    static const int POOL_SIZE = 5;
    static const int TOTAL_PARTICLES = PARTICLES_PER_SPAWN * POOL_SIZE;

    int spawnOffset = 0;

    std::vector<WaterParticle> waterParticles;
    std::vector<uint8_t> pixelBuffer;

    void init() override;
    void stepWaterComputeShader(float deltaTime, int enemyCount);
    void uploadParticlesCompShader();
};