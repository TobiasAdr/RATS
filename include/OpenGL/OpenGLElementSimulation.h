#include "../../include/Types.h"
#pragma once
#include <cstdint>
#include <iostream>
#include <vector>
#include <cmath>
#include <numbers>
#include <fstream>
#include <string>

class OpenGLElementSimulation
{

public:

    int W = 1920;
    int H = 1080;

    
    OpenGLElementSimulation(std::vector<sf::CircleShape>& colliders, int width, int height);

    virtual void init() = 0;

    float* getVertices() { return this->vertices; }

    virtual void update(float deltaTime);

    void spawnSand(int x, int y);
    void spawnSandChunk(int x, int y);
    void shootSand(float directionx, float directiony);
    void shootSandCone(float playerX, float playerY, float attackdirX, float attackdirY);
    void spawnRaySand(int prevx, int prevy, int x, int y);

    void spawnWater(int x, int y);

    const uint8_t* getPixels();
    virtual GLuint getTexture() const { return sandTexture; }

protected:

    GLuint loadComputeShader(const std::string& path);

    void deleteDeadSand();

    void collision();

    std::vector<sf::CircleShape>& colliders;

    std::vector<std::pair<int, int>> prevRayPositions;
    int prevX, prevY = 0;

    int dirtyMinX, dirtyMinY;
    int dirtyMaxX, dirtyMaxY;
    bool anyDirty = false;

    std::vector<SandRay> rays;

    static const int CHUNK_SIZE = 400;
    unsigned int CHUNKS_X;
    unsigned int CHUNKS_Y;

    std::vector<Chunk> chunks;

    int frameCount;

    void initSmallGrid(float left, float right, float top, float bottom);

    float vertices[24];

    GLuint sandTexture;

    std::vector<GridParticle> grid;

    std::vector<uint8_t> pixelBuffer;

    void uploadGrid();
    void uploadGridChunk();
    void step();
    void step2();
    void stepChunk();
    int getChunkIndex(int x, int y) const {
        int chunkX = x / CHUNK_SIZE;
        int chunkY = y / CHUNK_SIZE;
        return chunkY * CHUNKS_X + chunkX;
    }
    void updateChunkActivity();

    void updateRays(float deltaTime);
    void drawRaysToBuffer();

    void stepWater(int i);
};