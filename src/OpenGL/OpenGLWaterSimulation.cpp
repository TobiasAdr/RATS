#include "../../include/OpenGL/OpenGLWaterSimulation.h"

float elapsedTime = 0.0f;

OpenGLWaterSimulation::OpenGLWaterSimulation(std::vector<sf::CircleShape>& colliders, int width, int height)
    : OpenGLElementSimulation(colliders, width, height)
{
    pixelBuffer.resize(W * H * 4);
}

void OpenGLWaterSimulation::init() {

    glGenTextures(1, &waterTexture);
    glBindTexture(GL_TEXTURE_2D, waterTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, W, H,
        0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenBuffers(1, &particleSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
        sizeof(WaterParticle) * TOTAL_PARTICLES,
        nullptr,
        GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleSSBO);

    this->spawnProgram = loadComputeShader("shaders/water_spawn.comp");
    this->stepProgram = loadComputeShader("shaders/water_step.comp");
    this->waterRenderCompProgram = loadComputeShader("shaders/uploadParticles.comp");
    this->clearCompProgram = loadComputeShader("shaders/clear.comp");
}

void OpenGLWaterSimulation::spawnWaterComputeShader(float x, float y, float dirX, float dirY) {

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleSSBO);
    glUseProgram(this->spawnProgram);

    glUniform1f(glGetUniformLocation(this->spawnProgram, "elapsedTime"), elapsedTime);
    glUniform2f(glGetUniformLocation(this->spawnProgram, "spawnOrigin"), x, y);
    glUniform2f(glGetUniformLocation(this->spawnProgram, "direction"), dirX, dirY);
    glUniform1i(glGetUniformLocation(this->spawnProgram, "particleCount"), PARTICLES_PER_SPAWN);
    glUniform1i(glGetUniformLocation(this->spawnProgram, "spawnOffset"), spawnOffset);

    glDispatchCompute(1, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    spawnOffset = (spawnOffset + PARTICLES_PER_SPAWN) % TOTAL_PARTICLES;
}

void OpenGLWaterSimulation::stepWaterComputeShader(float deltaTime, int enemyCount) {

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleSSBO);
    elapsedTime += deltaTime;

    glUseProgram(this->stepProgram);
    glUniform1f(glGetUniformLocation(this->stepProgram, "elapsedTime"), elapsedTime);
    glUniform1i(glGetUniformLocation(this->stepProgram, "particleCount"), TOTAL_PARTICLES);
    glUniform1i(glGetUniformLocation(this->stepProgram, "enemyCount"), enemyCount);

    glDispatchCompute((TOTAL_PARTICLES + 63) / 64, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void OpenGLWaterSimulation::uploadParticlesCompShader() {
   
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleSSBO);
    glBindImageTexture(1, waterTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);

    static int frameCounter = 0;
    if (++frameCounter % 3 == 0) {
        glUseProgram(clearCompProgram);
        glDispatchCompute((W + 15) / 16, (H + 15) / 16, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    }

    glUseProgram(waterRenderCompProgram);
    glUniform1i(glGetUniformLocation(waterRenderCompProgram, "particleCount"), TOTAL_PARTICLES);
    glDispatchCompute((TOTAL_PARTICLES + 63) / 64, 1, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void OpenGLWaterSimulation::update(float deltaTime, int enemyCount) {

    this->stepWaterComputeShader(deltaTime, enemyCount);
    this->uploadParticlesCompShader();
}