#include "../../include/OpenGL/OpenGLFireSimulation.h"

OpenGLFireSimulation::OpenGLFireSimulation(std::vector<sf::CircleShape>& colliders, int width, int height)
    : OpenGLElementSimulation(colliders, width, height)
{
}

void OpenGLFireSimulation::init() {

    glGenTextures(1, &fireTexture);
    glBindTexture(GL_TEXTURE_2D, fireTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, W, H,
        0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenBuffers(1, &fireParticleSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, fireParticleSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
        sizeof(FirePixel) * 64 * 36 * POOL_SIZE,
        nullptr,
        GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, fireParticleSSBO);

    this->colorProgram = loadComputeShader("shaders/colorFire.comp");
    this->explodeProgram = loadComputeShader("shaders/explode.comp");
}

void OpenGLFireSimulation::spawnFire(int posX, int posY, float dirX, float dirY) {

    FireParticleCPU& p = particles[spawnOffset];
    p.prevX = p.posX;
    p.prevY = p.posY;
    p.posX = posX;
    p.posY = posY;
    p.dirX = dirX;
    p.dirY = dirY;
    p.startX = posX;
    p.startY = posY;
    p.life = 200;
    p.flying = 1;

    spawnOffset = (spawnOffset + 1) % POOL_SIZE;
}

void OpenGLFireSimulation::stepFire(float deltaTime) {

    float speed = 50.0f;

    for (int i = 0; i < POOL_SIZE; i++) {
        FireParticleCPU& p = particles[i];

        if (p.flying == 0) continue;

        if (p.life <= 0) {
            p.life = 0;
            p.flying = 0;
            continue;
        }

        p.prevX = p.posX;
        p.prevY = p.posY;
        p.posX += p.dirX * speed * deltaTime * 5;
        p.posY += p.dirY * speed * deltaTime * 5;
        p.life--;
    }
}

void OpenGLFireSimulation::colorFire() {
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, fireParticleSSBO);
    glBindImageTexture(2, fireTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
    glUseProgram(this->colorProgram);

    for (int i = 0; i < POOL_SIZE; i++) {
        FireParticleCPU& p = particles[i];
        if (p.flying == 0) continue;

        glUniform1f(glGetUniformLocation(this->colorProgram, "positionX"), p.posX);
        glUniform1f(glGetUniformLocation(this->colorProgram, "positionY"), p.posY);
        glUniform1f(glGetUniformLocation(this->colorProgram, "prevX"), p.prevX);
        glUniform1f(glGetUniformLocation(this->colorProgram, "prevY"), p.prevY);
        glUniform1i(glGetUniformLocation(this->colorProgram, "ssboOffset"), i * 64 * 36);

        glDispatchCompute(1, 1, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
    }
}

void OpenGLFireSimulation::explode(float deltaTime, int enemyCount) {
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, fireParticleSSBO);

    glBindImageTexture(2, fireTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
    glUseProgram(this->explodeProgram);

    for (int i = 0; i < POOL_SIZE; i++) {
        FireParticleCPU& p = particles[i];
        if (p.flying != 0) continue; 
        if (p.life != 0) continue;   

        glUniform1f(glGetUniformLocation(this->explodeProgram, "deltaTime"), deltaTime);
        glUniform1i(glGetUniformLocation(this->explodeProgram, "enemyCount"), enemyCount);
        glUniform1i(glGetUniformLocation(this->explodeProgram, "ssboOffset"), i * 64 * 36);

        int particleCount = 64 * 36; 
        glDispatchCompute((particleCount + 63) / 64, 1, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
    }
}

void OpenGLFireSimulation::update(float deltaTime, int enemyCount) {

    colorFire();
    explode(deltaTime, enemyCount);
    stepFire(deltaTime);
}