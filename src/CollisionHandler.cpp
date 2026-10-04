#include "../include/CollisionHandler.h"
#include <algorithm>
#include <iostream>

CollisionHandler::CollisionHandler() {};

sf::Vector2f CollisionHandler::frameCollision(const sf::Vector2f &position, const sf::Vector2f &size, const sf::FloatRect &bounds)
{
    sf::Vector2f newPosition = position;

    if (position.x < bounds.position.x)
    {
        newPosition.x = bounds.position.x;
    }

    if (position.x + size.x > bounds.position.x + bounds.size.x)
    {
        newPosition.x = bounds.position.x + bounds.size.x - size.x;
    }

    if (position.y + size.y >= bounds.position.y + bounds.size.y)
    {
        newPosition.y = bounds.position.y + bounds.size.y - size.y;
    }

    return newPosition;
};

sf::Vector2f CollisionHandler::platformCollision(const sf::Vector2f &position, const sf::Vector2f size, const sf::Vector2f velocity, std::vector<Platform> &platforms)
{
    sf::Vector2f newPosition = position;
    sf::FloatRect characterBounds(position, size);

    for (auto &platform : platforms)
    {
        sf::FloatRect platformBounds(platform.getPosition(), platform.getSize());

        float checkPlatformTop = abs((characterBounds.position.y + size.y) - platformBounds.position.y);
        float checkPlatformBottom = abs(characterBounds.position.y - (platformBounds.position.y + platformBounds.size.y));

        if (characterBounds.findIntersection(platformBounds))
        {
            std::cout << checkPlatformTop << std::endl;

            if (velocity.y > 0)
            {
                if (characterBounds.position.x + size.x > platformBounds.position.x && characterBounds.position.x < platformBounds.position.x + platformBounds.size.x && checkPlatformTop <= 5.0f)
                {
                    newPosition.y = platformBounds.position.y - size.y;
                }
                else if (velocity.x > 0)
                {
                    newPosition.x = platformBounds.position.x - size.x;
                }
                else if (velocity.x < 0)
                {
                    newPosition.x = platformBounds.position.x + platformBounds.size.x;
                }
            }
            else if (velocity.y < 0)
            {
                std::cout << characterBounds.position.y << " " << platformBounds.position.y + platformBounds.size.y << std::endl;

                if (checkPlatformBottom < 5.0f)
                {
                    newPosition.y = platformBounds.position.y + platformBounds.size.y;
                }
                else if (velocity.x > 0)
                {
                    newPosition.x = platformBounds.position.x - size.x;
                }
                else if (velocity.x < 0)
                {
                    newPosition.x = platformBounds.position.x + platformBounds.size.x;
                }
            }
            else if (velocity.y == 0.0f)
            {
                if (velocity.x > 0)
                {
                    newPosition.x = platformBounds.position.x - size.x;
                }
                else if (velocity.x < 0)
                {
                    newPosition.x = platformBounds.position.x + platformBounds.size.x;
                }
            }
        }
    }

    return newPosition;
};

void CollisionHandler::enemyHit(std::vector<Enemy> &enemies, Player &player, const sf::Vector2f &attackDirection, float hitRadius)
{
    float projection = 0.f;

    sf::Vector2f playerPos = player.getPosition();
    float swordLength = player.getSwordLength();
    sf::Vector2f middlePlayer = playerPos + player.getSize() / 2.f;

    for (auto &enemy : enemies)
    {
        sf::Vector2f enemyPos = enemy.getPosition();

        sf::Vector2f enemyMiddle = enemyPos + enemy.getSize() / 2.f;

        sf::Vector2f toEnemy = enemyMiddle - middlePlayer;

        projection = toEnemy.x * attackDirection.x + toEnemy.y * attackDirection.y;

        if (projection > 1 && projection <= swordLength)
        {
            sf::Vector2f closestPoint = middlePlayer + attackDirection * projection;

            float dx = enemyMiddle.x - closestPoint.x;
            float dy = enemyMiddle.y - closestPoint.y;

            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < hitRadius)
            {
                enemy.removeHP(50);
                enemy.bounceBack();
                std::cout << enemy.getHP() << std::endl;
            }
        }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),

                                 [](Enemy &e)
                                 {
                                     return e.getHP() <= 0;
                                 }),

                  enemies.end());
};

void CollisionHandler::playerEnemyCollision(std::vector<Enemy> &enemies, Player &player)
{
    sf::Vector2f playerPos = player.getPosition();
    float hitRadius = 50;

    for (auto &enemy : enemies)
    {
        sf::Vector2f enemyPos = enemy.getPosition();
        float deltaX = enemyPos.x - playerPos.x;
        float deltaY = std::abs(enemyPos.y - playerPos.y);

        if (deltaY < hitRadius)
            if (deltaX >= 0)
            {
                if (deltaX <= hitRadius + player.getSize().x / 2.f)
                {
                    std::cout << "Hit from right" << std::endl;
                }
            }
            else
            {
                if (deltaX > ((player.getSize().x / 2.f) - hitRadius))
                {
                    std::cout << "Hit from left" << std::endl;
                }
            }
    }
}

bool CollisionHandler::fireballHitEnemy(Fireball fireball, Enemy enemy)
{
    sf::FloatRect fireballBounds = fireball.shape.getGlobalBounds();
    sf::FloatRect enemyBounds = enemy.getBody().getGlobalBounds();

    if (fireballBounds.findIntersection(enemyBounds))
        return true;

    return false;
};

void CollisionHandler::objectCollision(std::vector<sf::CircleShape> &objects, Character &character)
{
    sf::FloatRect characterBounds(character.getPosition(), character.getSize());

    sf::Vector2f charPos = character.getPosition();
    sf::Vector2f charSize = character.getSize();

    sf::Vector2f velocity = character.getVelocity();

    for (auto &object : objects)
    {
        sf::FloatRect objectBounds = object.getGlobalBounds();

        sf::Vector2f objPos = objectBounds.position;
        sf::Vector2f objSize = objectBounds.size;

        if (objectBounds.findIntersection(characterBounds))
        {
            if (velocity.y < 0)
            {
                character.setPosition(sf::Vector2f(charPos.x, objPos.y + objSize.y));
            }

            if (velocity.y > 0)
            {
                character.setPosition(sf::Vector2f(charPos.x, objPos.y - charSize.y));
            }

            if (velocity.x > 0)
            {
                character.setPosition(sf::Vector2f(objPos.x - charSize.x, charPos.y));
            }

            if (velocity.x < 0)
            {
                character.setPosition(sf::Vector2f(objPos.x + objSize.x, charPos.y));
            }
        }
    }
};
