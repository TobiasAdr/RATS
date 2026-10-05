#include "../include/EntityManager.h"
#include <iostream>

EntityManager::EntityManager()
{

    this->setupMap();
    this->loadTerrain();

    this->waterSimulator = std::make_unique<OpenGLWaterSimulation>(this->colliders, 1920, 1080);
    this->fireSimulator = std::make_unique<OpenGLFireSimulation>(this->colliders, 1920, 1080);
    this->rodsSimulator = std::make_unique<OpenGLBouncingRods>(this->colliders, 1920, 1080);
}

void EntityManager::initEnemies()
{

    enemyManager.init("resources/ratGPT.png");
}
void EntityManager::spawnEnemies(float deltaTime)
{
    spawnTimer += deltaTime;

    if (spawnTimer >= spawnInterval)
    {
        spawnTimer = 0.f;

        float x = 700.f + (rand() % 400);
        float y = 700.f + (rand() % 400);

        enemyManager.spawnEnemy(x, y);
    }
}

void EntityManager::initPlayer(int windowWidth, int windowHeight)
{
    this->playerIdleTexture.loadFromFile("resources/Wizard4.png");
    this->playerSprite.emplace(this->playerIdleTexture);

    this->playerSprite->setTextureRect(
        sf::IntRect({0, 0}, {1145, 1201}));

    this->playerSprite->setScale(sf::Vector2f(0.02f, 0.02f));

    this->player.setPosition(sf::Vector2f(
        windowWidth / 2.3f,
        windowHeight / 1.3f));

    this->playerSprite->setPosition(this->player.getPosition());
}

void EntityManager::setupMap()
{
    this->layerObjects = this->world.loadMap("resources/blackmap2.csv");
}

void EntityManager::loadTerrain()
{

    for (int y = 0; y < layerObjects.size(); y++)
    {
        for (int x = 0; x < layerObjects[y].size(); x++)
        {

            int tileID = layerObjects[y][x];

            if (tileID == 62)
            {
                sf::CircleShape circle(2.5f);

                circle.setOrigin(sf::Vector2f(16.f, 16.f));

                circle.setPosition(sf::Vector2f(
                    1600 / 2 + (x - y) * (TILE_W / 2.f) + 20,
                    1200 / 2 + (x + y) * (TILE_H / 4.f) + 20));

                circle.setFillColor(
                    sf::Color(255, 0, 0, 100));

                colliders.push_back(circle);
            }
        }
    }
}

void EntityManager::updatePlayer(float deltaTime)
{

    this->playerSprite->setPosition(this->player.getPosition());
    std::cout << this->player.getHP() << std::endl;

    this->player.setRunning(false);

    this->player.update(deltaTime);

    this->player.setVelocityX(0.0f);
    this->player.setVelocityY(0.0f);

    if (this->enemyManager.processPlayerHit(player.getPosition().x, player.getPosition().y))
    {

        this->player.removeHP(1);
    }
}

void EntityManager::updateEnemies(float deltaTime)
{

    spawnEnemies(deltaTime);
}

void EntityManager::update(float deltaTime)
{

    this->updatePlayer(deltaTime);
    this->updateEnemies(deltaTime);
    this->firePlace.update(deltaTime);

    this->enemyManager.uploadEnemyPosition();
    int count = enemyManager.getAliveCount();
    this->waterSimulator->update(deltaTime, count);
    this->fireSimulator->update(deltaTime, count);
    this->rodsSimulator->update(deltaTime, count);
    this->enemyManager.processHits();

    this->enemyManager.update(deltaTime, player.getPosition().x, player.getPosition().y);
};

std::optional<sf::Sprite> EntityManager::getPlayerSprite()
{

    return this->playerSprite;
};
void EntityManager::init(int width, int height)
{

    this->waterSimulator = std::make_unique<OpenGLWaterSimulation>(this->colliders, width, height);
    this->fireSimulator = std::make_unique<OpenGLFireSimulation>(this->colliders, width, height);
    this->rodsSimulator = std::make_unique<OpenGLBouncingRods>(this->colliders, width, height);

    this->initEnemies();
    this->initPlayer(width, height);
}

sf::Vector2f EntityManager::getPlayerPos()
{

    return this->player.getPosition();
};

Player &EntityManager::getPlayer()
{

    return this->player;
};
