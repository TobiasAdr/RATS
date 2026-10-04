#include "../../include/OpenGL/OpenGLElementSimulation.h"
#include <algorithm>

OpenGLElementSimulation::OpenGLElementSimulation(std::vector<sf::CircleShape> &colliders, int width, int height)
    : colliders(colliders), W(width), H(height)
{

    CHUNKS_X = W / CHUNK_SIZE;
    CHUNKS_Y = H / CHUNK_SIZE;

    chunks.resize(CHUNKS_X * CHUNKS_Y);

    dirtyMinX = W;
    dirtyMinY = H;
    dirtyMaxX = 0;
    dirtyMaxY = 0;
    anyDirty = false;

    grid.resize(W * H);

    for (auto &cell : grid)
    {
        cell.type = Cell::Empty;
        cell.life = 0;
    }

    pixelBuffer.resize(W * H * 4);
}

void OpenGLElementSimulation::init()
{

    glGenTextures(1, &sandTexture);

    glBindTexture(GL_TEXTURE_2D, sandTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, W, H,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void OpenGLElementSimulation::uploadGrid()
{

    for (int i = 0; i < W * H; i++)
    {
        bool isSand = grid[i].type == Cell::Sand;
        pixelBuffer[i * 4 + 0] = isSand ? 194 : 0;
        pixelBuffer[i * 4 + 1] = isSand ? 178 : 0;
        pixelBuffer[i * 4 + 2] = isSand ? 80 : 0;
        pixelBuffer[i * 4 + 3] = isSand ? 255 : 0;
    }

    glBindTexture(GL_TEXTURE_2D, sandTexture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, H,
                    GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer.data());
}

void OpenGLElementSimulation::uploadGridChunk()
{
    if (!anyDirty)
        return;

    int rectW = dirtyMaxX - dirtyMinX + 1;
    int rectH = dirtyMaxY - dirtyMinY + 1;

    std::vector<uint8_t> pixels(rectW * rectH * 4);
    for (int y = dirtyMinY; y <= dirtyMaxY; y++)
    {
        for (int x = dirtyMinX; x <= dirtyMaxX; x++)
        {
            int i = (y - dirtyMinY) * rectW + (x - dirtyMinX);
            bool isSand = grid[y * W + x].type == Cell::Sand;
            pixels[i * 4 + 0] = isSand ? 194 : 0;
            pixels[i * 4 + 1] = isSand ? 178 : 0;
            pixels[i * 4 + 2] = isSand ? 80 : 0;
            pixels[i * 4 + 3] = isSand ? 255 : 0;
        }
    }

    
    for (auto &ray : rays)
    {
        int x = (int)ray.x;
        int y = (int)ray.y;
        if (x >= dirtyMinX && x <= dirtyMaxX && y >= dirtyMinY && y <= dirtyMaxY)
        {
            int i = (y - dirtyMinY) * rectW + (x - dirtyMinX);
            pixels[i * 4 + 0] = 194;
            pixels[i * 4 + 1] = 178;
            pixels[i * 4 + 2] = 80;
            pixels[i * 4 + 3] = 255;
        }
    }

    glBindTexture(GL_TEXTURE_2D, sandTexture);
    glTexSubImage2D(GL_TEXTURE_2D, 0,
                    dirtyMinX, dirtyMinY,
                    rectW, rectH,
                    GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    dirtyMinX = W;
    dirtyMinY = H;
    dirtyMaxX = 0;
    dirtyMaxY = 0;
    anyDirty = false;
}

void OpenGLElementSimulation::spawnSand(int x, int y)
{

    if (x >= 0 && x < W && y >= 0 && y < H)
    {

        grid[y * W + x].type = Cell::Sand; 
        grid[y * W + x].life = 10;

        chunks[getChunkIndex(x, y)].active = true;
        chunks[getChunkIndex(x, y)].sleeping = false;
    }
}

void OpenGLElementSimulation::spawnRaySand(int prevx, int prevy, int x, int y)
{

    if (x >= 0 && x < W && y >= 0 && y < H)
    {

        int i = y * W + x;
        if (prevy < y)
        {
            for (int py = prevy; py < y; py++)
            {
                int interi = py * W + x;
                grid[interi].type = Cell::Sand;
                grid[interi].life = 10;
            }
        }
        else
        {

            for (int py = prevy; py > y; py--)
            {
                int interi = py * W + x;
                grid[interi].type = Cell::Sand;
                grid[interi].life = 10;
            }
        }

        chunks[getChunkIndex(x, y)].active = true;
        chunks[getChunkIndex(x, y)].sleeping = false;

        dirtyMinX = std::min(dirtyMinX, x);
        dirtyMinY = std::min(dirtyMinY, y);
        dirtyMaxX = std::max(dirtyMaxX, x);
        dirtyMaxY = std::max(dirtyMaxY, y);
        anyDirty = true;
    }
};

void OpenGLElementSimulation::spawnSandChunk(int x, int y)
{
    if (x >= 0 && x < W && y >= 0 && y < H)
    {
        grid[y * W + x].type = Cell::Sand;
        grid[y * W + x].life = 50;
        grid[y * W + x].sleeping = false;

        chunks[getChunkIndex(x, y)].active = true;
        chunks[getChunkIndex(x, y)].sleeping = false;

        dirtyMinX = std::min(dirtyMinX, x);
        dirtyMinY = std::min(dirtyMinY, y);
        dirtyMaxX = std::max(dirtyMaxX, x);
        dirtyMaxY = std::max(dirtyMaxY, y);
        anyDirty = true;
    }
}


void OpenGLElementSimulation::step()
{

    for (int y = H - 2; y >= 0; y--)
    {

        for (int x = 0; x < W; x++)
        {

            if (grid[y * W + x].type != Cell::Sand)
                continue;

            if (grid[(y + 1) * W + x].type == Cell::Empty)
            { 
                grid[(y + 1) * W + x].type = Cell::Sand;
                grid[y * W + x].type = Cell::Empty;
            }

            else if (x > 0 && grid[(y + 1) * W + (x - 1)].type == Cell::Empty)
            { 
                grid[(y + 1) * W + (x - 1)].type = Cell::Sand;
                grid[y * W + x].type = Cell::Empty;
            }

            else if (x < W - 1 && grid[(y + 1) * W + (x + 1)].type == Cell::Empty)
            {
                grid[(y + 1) * W + (x + 1)].type = Cell::Sand;
                grid[y * W + x].type = Cell::Empty;
            }
        }
    }
};

void OpenGLElementSimulation::collision()
{

    for (auto &collider : this->colliders)
    {
    }
}

void OpenGLElementSimulation::step2()
{

    for (int y = H - 2; y >= 0; y--)
    {

        for (int x = 0; x < W; x++)
        {

            if (grid[y * W + x].type != Cell::Sand)
                continue;
            if (grid[y * W + x].life <= 0)
                continue;

            grid[y * W + x].life--;

            if (grid[(y + 1) * W + x].type == Cell::Empty)
            { 
                grid[(y + 1) * W + x].type = Cell::Sand;
                grid[(y + 1) * W + x].life = grid[y * W + x].life; 
                grid[y * W + x].type = Cell::Empty;
                grid[y * W + x].life = 0;
            }

            else if (x > 0 && grid[(y + 1) * W + (x - 1)].type == Cell::Empty)
            { 
                grid[(y + 1) * W + (x - 1)].type = Cell::Sand;
                grid[y * W + x].type = Cell::Empty;
            }

            else if (x < W - 1 && grid[(y + 1) * W + (x + 1)].type == Cell::Empty)
            {
                grid[(y + 1) * W + (x + 1)].type = Cell::Sand;
                grid[y * W + x].type = Cell::Empty;
            }
        }
    }
};

void OpenGLElementSimulation::stepChunk()
{

    auto markDirty = [&](int x, int y)
    {
        dirtyMinX = std::min(dirtyMinX, x);
        dirtyMinY = std::min(dirtyMinY, y);
        dirtyMaxX = std::max(dirtyMaxX, x);
        dirtyMaxY = std::max(dirtyMaxY, y);
        anyDirty = true;
    };

    for (int chunkY = CHUNKS_Y - 1; chunkY >= 0; chunkY--)
    {
        for (int chunkX = 0; chunkX < CHUNKS_X; chunkX++)
        {

            Chunk &chunk = chunks[chunkY * CHUNKS_X + chunkX];
            if (!chunk.active || chunk.sleeping)
                continue;

            int startX = chunkX * CHUNK_SIZE;
            int startY = chunkY * CHUNK_SIZE;
            int endX = startX + CHUNK_SIZE;
            int endY = startY + CHUNK_SIZE;

            bool anyMoved = false;

            for (int y = endY - 2; y >= startY; y--)
            {
                for (int x = startX; x < endX; x++)
                {

                    if (grid[y * W + x].type != Cell::Sand)
                        continue;
                    if (grid[y * W + x].sleeping)
                        continue;
                    if (grid[y * W + x].life <= 0)
                        continue;

                    bool moved = false;
                    grid[y * W + x].life--;

                    if (grid[(y + 1) * W + x].type == Cell::Empty)
                    {
                        grid[(y + 1) * W + x].type = Cell::Sand;
                        grid[(y + 1) * W + x].sleeping = false;
                        grid[(y + 1) * W + x].life = grid[y * W + x].life;
                        grid[y * W + x].type = Cell::Empty;
                        grid[y * W + x].sleeping = false;
                        grid[y * W + x].life = 0;
                        markDirty(x, y);
                        markDirty(x, y + 1);
                        moved = true;

                        if (y + 1 >= startY + CHUNK_SIZE)
                        {
                            int belowChunk = getChunkIndex(x, y + 1);
                            chunks[belowChunk].active = true;
                            chunks[belowChunk].sleeping = false;
                        }
                    }
                    else if (x > 0 && grid[(y + 1) * W + (x - 1)].type == Cell::Empty)
                    {
                        grid[(y + 1) * W + (x - 1)].type = Cell::Sand;
                        grid[(y + 1) * W + (x - 1)].sleeping = false;
                        grid[(y + 1) * W + (x - 1)].life = grid[y * W + x].life;
                        grid[y * W + x].type = Cell::Empty;
                        grid[y * W + x].sleeping = false;
                        grid[y * W + x].life = 0;
                        markDirty(x, y);
                        markDirty(x - 1, y + 1);
                        moved = true;

                        if (y + 1 >= startY + CHUNK_SIZE || x - 1 < startX)
                        {
                            int belowChunk = getChunkIndex(x - 1, y + 1);
                            chunks[belowChunk].active = true;
                            chunks[belowChunk].sleeping = false;
                        }
                    }
                    else if (x < W - 1 && grid[(y + 1) * W + (x + 1)].type == Cell::Empty)
                    {
                        grid[(y + 1) * W + (x + 1)].type = Cell::Sand;
                        grid[(y + 1) * W + (x + 1)].sleeping = false;
                        grid[(y + 1) * W + (x + 1)].life = grid[y * W + x].life;
                        grid[y * W + x].type = Cell::Empty;
                        grid[y * W + x].sleeping = false;
                        grid[y * W + x].life = 0;
                        markDirty(x, y);
                        markDirty(x + 1, y + 1);
                        moved = true;

                        if (y + 1 >= startY + CHUNK_SIZE || x + 1 >= startX + CHUNK_SIZE)
                        {
                            int belowChunk = getChunkIndex(x + 1, y + 1);
                            chunks[belowChunk].active = true;
                            chunks[belowChunk].sleeping = false;
                        }
                    }

                    if (moved)
                        anyMoved = true;
                    else
                        grid[y * W + x].sleeping = true;
                }
            }

            if (!anyMoved)
                chunk.sleeping = true;
        }
    }
}

void OpenGLElementSimulation::updateChunkActivity()
{
    for (int chunkY = 0; chunkY < CHUNKS_Y; chunkY++)
    {
        for (int chunkX = 0; chunkX < CHUNKS_X; chunkX++)
        {

            int startX = chunkX * CHUNK_SIZE;
            int startY = chunkY * CHUNK_SIZE;

            bool hasSand = false;
            for (int y = startY; y < startY + CHUNK_SIZE && !hasSand; y++)
            {
                for (int x = startX; x < startX + CHUNK_SIZE && !hasSand; x++)
                {
                    if (grid[y * W + x].type == Cell::Sand)
                        hasSand = true;
                }
            }

            if (!hasSand)
                chunks[chunkY * CHUNKS_X + chunkX].active = false;
        }
    }
}

void OpenGLElementSimulation::shootSandCone(float playerX, float playerY, float attackdirX, float attackdirY)
{

    int numRays = 30;
    float spreadAngle = 30.f; 
    float speed = 4.f;

    for (int i = 0; i < numRays; i++)
    {
        
        float t = (float)i / (numRays - 1); 
        float angle = (-spreadAngle / 2 + t * spreadAngle) * 3.14159f / 180.f;

        float cos_a = cos(angle);
        float sin_a = sin(angle);
        float rx = attackdirX * cos_a - attackdirY * sin_a;
        float ry = attackdirX * sin_a + attackdirY * cos_a;

        SandRay ray;
        ray.x = playerX;
        ray.y = playerY;
        ray.vx = rx * speed; 
        ray.vy = ry * speed;
        ray.life = 10.f; 
        rays.push_back(ray);
    }
};

void OpenGLElementSimulation::updateRays(float deltaTime)
{
    for (auto &ray : rays)
    {
        
        int prevX = (int)ray.x;
        int prevY = (int)ray.y;
        if (prevX >= 0 && prevX < W && prevY >= 0 && prevY < H)
        {
            dirtyMinX = std::min(dirtyMinX, prevX);
            dirtyMinY = std::min(dirtyMinY, prevY);
            dirtyMaxX = std::max(dirtyMaxX, prevX);
            dirtyMaxY = std::max(dirtyMaxY, prevY);
            anyDirty = true;
        }

        bool hit = false;
        ray.x += ray.vx;
        ray.y += ray.vy;
        ray.life--;

        
        for (const auto &collider : colliders)
        {
            sf::Vector2f center = collider.getPosition();
            float radius = collider.getRadius() * 4;

            float dx = ray.x - center.x;
            float dy = ray.y - center.y;
            float distSq = dx * dx + dy * dy;

            if (distSq < radius * radius)
            {
                spawnSand((int)ray.x, (int)ray.y);
                ray.life = 0; 
                hit = true;
            }
        }

        if (hit)
            continue;

        int gridX = (int)ray.x;
        int gridY = (int)ray.y;

        if (gridX >= 0 && gridX < W && gridY >= 0 && gridY < H)
        {
            dirtyMinX = std::min(dirtyMinX, gridX);
            dirtyMinY = std::min(dirtyMinY, gridY);
            dirtyMaxX = std::max(dirtyMaxX, gridX);
            dirtyMaxY = std::max(dirtyMaxY, gridY);
            anyDirty = true;
        }

        if (ray.life <= 0)
        {
            spawnSand(gridX, gridY);
        }
    }

    rays.erase(
        std::remove_if(rays.begin(), rays.end(),
                       [](const SandRay &r)
                       { return r.life <= 0; }),
        rays.end());
}

void OpenGLElementSimulation::drawRaysToBuffer()
{
    for (auto &ray : rays)
    {
        int x = (int)ray.x;
        int y = (int)ray.y;
        if (x >= 0 && x < W && y >= 0 && y < H)
        {
            int i = y * W + x;
            pixelBuffer[i * 4 + 0] = 194;
            pixelBuffer[i * 4 + 1] = 178;
            pixelBuffer[i * 4 + 2] = 80;
            pixelBuffer[i * 4 + 3] = 255;

            dirtyMinX = std::min(dirtyMinX, x);
            dirtyMinY = std::min(dirtyMinY, y);
            dirtyMaxX = std::max(dirtyMaxX, x);
            dirtyMaxY = std::max(dirtyMaxY, y);
            anyDirty = true;
        }
    }
}

void OpenGLElementSimulation::update(float deltaTime)
{

    stepChunk();
    this->updateRays(deltaTime);
    uploadGridChunk();
}


void OpenGLElementSimulation::spawnWater(int x, int y)
{

    if (x >= 0 && x < W && y >= 0 && y < H)
    {
        for (int i = 0; i < 5; i++)
        {

            grid[y * W + x + 1].type = Cell::Water;
            

            chunks[getChunkIndex(x, y)].active = true;
            chunks[getChunkIndex(x, y)].sleeping = false;
        }
    }
};

void stepWater(int i) {

};

GLuint OpenGLElementSimulation::loadComputeShader(const std::string &path)
{

    
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "Kunde inte öppna: " << path << std::endl;
        return 0;
    }

    std::string src((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());

    const char *c_src = src.c_str();

    
    GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, &c_src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "Kompileringsfel i " << path << ": " << log << std::endl;
        return 0;
    }

    
    GLuint program = glCreateProgram();
    glAttachShader(program, shader);
    glLinkProgram(program);

    return program;
};