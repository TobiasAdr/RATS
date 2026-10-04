#include "../../include/OpenGL/EnemyManagerOpenGL.h"
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

struct InstanceData
{
    float x, y;
    float hit;
};

static const float QUAD[] = {

    -16.f,
    -16.f,
    0.f,
    1.f,
    16.f,
    -16.f,
    1.f,
    1.f,
    16.f,
    16.f,
    1.f,
    0.f,

    -16.f,
    -16.f,
    0.f,
    1.f,
    16.f,
    16.f,
    1.f,
    0.f,
    -16.f,
    16.f,
    0.f,
    0.f,
};

EnemyManagerOpenGL::EnemyManagerOpenGL() {}

EnemyManagerOpenGL::~EnemyManagerOpenGL()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteBuffers(1, &instanceVBO);
    glDeleteTextures(1, &texture);
}

void EnemyManagerOpenGL::init(const std::string &texturePath)
{

    sf::Image img;
    if (!img.loadFromFile(texturePath))
    {
        std::cerr << "[EnemyManagerOpenGL] Kunde inte ladda: " << texturePath << "\n";
    }
    auto size = img.getSize();

    glGenBuffers(1, &enemySSBO);
    glGenBuffers(1, &hitSSBO);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 size.x, size.y, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD), QUAD, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);
}

void EnemyManagerOpenGL::spawnEnemy(float x, float y, float hp)
{
    enemies.push_back({x, y, hp, true});
    rebuildInstanceBuffer();
}

void EnemyManagerOpenGL::update(float deltaTime, float playerX, float playerY)
{

    for (auto &e : enemies)
    {
        float dx = playerX - e.x;
        float dy = playerY - e.y;

        float len = std::sqrt(dx * dx + dy * dy);
        if (len > 0.f)
        {
            dx /= len;
            dy /= len;
        }

        float speed = 50.f;
        e.x += dx * speed * deltaTime;
        e.y += dy * speed * deltaTime;
    }
}

void EnemyManagerOpenGL::render(GLuint shaderProgram, sf::RenderWindow &window, float firePosX, float firePosY)
{

    int alive = 0;
    for (auto &e : enemies)
        if (e.alive)
            alive++;
    if (alive == 0)
        return;

    rebuildInstanceBuffer();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(shaderProgram);

    sf::Transform sfTransform = window.getView().getTransform();
    const float *sfMatrix = sfTransform.getMatrix();
    glUniformMatrix4fv(
        glGetUniformLocation(shaderProgram, "uProjection"),
        1, GL_FALSE, sfMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shaderProgram, "uTexture"), 0);
    glUniform2f(glGetUniformLocation(shaderProgram, "uFirePos"), firePosX, firePosY);
    glUniform3f(glGetUniformLocation(shaderProgram, "uAmbient"), 0.05f, 0.05f, 0.06f);

    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, alive);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
}

int EnemyManagerOpenGL::checkHit(float px, float py, float radius)
{

    for (int i = 0; i < (int)enemies.size(); i++)
    {
        if (!enemies[i].alive)
            continue;
        float dx = px - enemies[i].x;
        float dy = py - enemies[i].y;
        if (dx * dx + dy * dy < radius * radius)
            return i;
    }

    return -1;
}

void EnemyManagerOpenGL::dealDamage(int index, float damage)
{

    if (index < 0 || index >= (int)enemies.size())
        return;

    EnemyOpenGL &e = enemies[index];

    if (!e.alive)
        return;

    e.hp -= damage;
    std::cout << e.hp << std::endl;
    if (e.hp <= 0.f)
    {
        e.alive = false;

        score += 100;

        rebuildInstanceBuffer();
    }
}

void EnemyManagerOpenGL::rebuildInstanceBuffer()
{
    std::vector<InstanceData> instances;

    for (auto &e : enemies)
    {
        if (!e.alive)
            continue;

        float dx = e.x - floorX;
        float dy = e.y - floorY;
        float hit = (dx * dx + dy * dy < 20.f * 20.f) ? 1.0f : 0.0f;

        instances.push_back({e.x, e.y, hit});
    }

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 instances.size() * sizeof(InstanceData),
                 instances.data(), GL_DYNAMIC_DRAW);
}

void EnemyManagerOpenGL::uploadEnemyPosition()
{
    std::vector<GPUEnemy> gpuEnemies;
    for (auto &e : enemies)
    {
        if (!e.alive)
            continue;
        gpuEnemies.push_back({e.x, e.y, {0, 0}});
    }

    if (gpuEnemies.empty())
        return;

    std::vector<int> hits(gpuEnemies.size(), 0);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, enemySSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
                 gpuEnemies.size() * sizeof(GPUEnemy),
                 gpuEnemies.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, enemySSBO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, hitSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
                 hits.size() * sizeof(int),
                 hits.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, hitSSBO);
}

void EnemyManagerOpenGL::processHits()
{
    int count = getAliveCount();
    if (count == 0)
        return;

    std::vector<int> hits(count);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, hitSSBO);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,
                       count * sizeof(int), hits.data());

    std::vector<int> aliveIndices;
    for (int i = 0; i < (int)enemies.size(); i++)
    {
        if (enemies[i].alive)
            aliveIndices.push_back(i);
    }

    for (int i = 0; i < count; i++)
    {
        if (hits[i] == 1)
        {
            int enemyIndex = aliveIndices[i];
            dealDamage(enemyIndex, 1.f);
        }
    }
}

bool EnemyManagerOpenGL::processPlayerHit(float playerX, float playerY)
{
    const float hitRadius = 10.f;

    for (auto &e : enemies)
    {
        if (!e.alive)
            continue;

        float dx = e.x - playerX;
        float dy = e.y - playerY;
        float distSq = dx * dx + dy * dy;

        if (distSq < hitRadius * hitRadius)
        {
            return true;
        }
    }
    return false;
}
