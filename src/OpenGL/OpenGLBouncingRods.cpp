#include "../../include/OpenGL/OpenGLBouncingRods.h"

OpenGLBouncingRods::OpenGLBouncingRods(std::vector<sf::CircleShape>& colliders, int width, int height)
    : OpenGLElementSimulation(colliders, width, height) 
{
}

void OpenGLBouncingRods::init() {

    glGenTextures(1, &rodTexture);
    glBindTexture(GL_TEXTURE_2D, rodTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, W, H,
        0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenBuffers(1, &rodParticleSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, rodParticleSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
        sizeof(PurpleRod) * POOL_SIZE,
        nullptr,
        GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, rodParticleSSBO);

    this->rodProgram = loadComputeShader("shaders/rod.comp");
    this->updateRodProgram = loadComputeShader("shaders/updateRods.comp");
    this->colorRodProgram = loadComputeShader("shaders/colorRods.comp");
}

void OpenGLBouncingRods::spawnRods(int posX, int posY, float dirX, float dirY) {

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, rodParticleSSBO);
    glBindImageTexture(2, rodTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
    glUseProgram(this->rodProgram);

    glUniform1f(glGetUniformLocation(this->rodProgram, "posX"), posX);
    glUniform1f(glGetUniformLocation(this->rodProgram, "posY"), posY);
    glUniform1f(glGetUniformLocation(this->rodProgram, "dirX"), dirX);
    glUniform1f(glGetUniformLocation(this->rodProgram, "dirY"), dirY);
    glUniform1i(glGetUniformLocation(this->rodProgram, "spawnOffset"), this->spawnOffset);

    glDispatchCompute(1, 1, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);

    this->spawnOffset = (this->spawnOffset + 30) % POOL_SIZE;
}

void OpenGLBouncingRods::updateRods(float deltaTime, int enemyCount) {

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, rodParticleSSBO);
    glBindImageTexture(2, rodTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
    glUseProgram(this->updateRodProgram);

    glUniform1f(glGetUniformLocation(this->updateRodProgram, "deltaTime"), deltaTime);
    glUniform1i(glGetUniformLocation(this->updateRodProgram, "enemyCount"), enemyCount);

    glDispatchCompute(POOL_SIZE / 64 + 1, 1, 1); 
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
}

void OpenGLBouncingRods::colorRods() {

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, rodParticleSSBO);
    glBindImageTexture(2, rodTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
    glUseProgram(this->colorRodProgram);

    glDispatchCompute(POOL_SIZE / 64 + 1, 1, 1); 
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
}

void OpenGLBouncingRods::update(float deltaTime, int enemyCount) {

    this->updateRods(deltaTime, enemyCount);
    this->colorRods();
}