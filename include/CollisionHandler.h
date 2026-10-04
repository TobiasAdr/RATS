#pragma once
#include "Platform.h"
#include "Enemy.h"
#include "Player.h"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class CollisionHandler
{

public:
	CollisionHandler();

	sf::Vector2f frameCollision(const sf::Vector2f& position, const sf::Vector2f& size, const sf::FloatRect& bounds);
	sf::Vector2f platformCollision(const sf::Vector2f& position, const sf::Vector2f size, const sf::Vector2f velocity, std::vector<Platform>& platforms);

	sf::Vector2f rayPlatformCollision(const sf::Vector2f& rayOrigin, float angle, std::vector<Platform>& platforms);

	void enemyHit(std::vector<Enemy>& enemies, Player& player, const sf::Vector2f& attackDirection, float hitRadius);
	void playerEnemyCollision(std::vector<Enemy>& enemies, Player& player);

	bool rayCollision();

	bool fireballHitEnemy(Fireball fireball, Enemy enemy);

	void objectCollision(std::vector<sf::CircleShape>& objects, Character& character);

	void platformCollision2(Character& character, std::vector<sf::CircleShape>& platforms, float deltaTime);

	float sweptAABB(
		const sf::Vector2f& pos, const sf::Vector2f& size,
		const sf::Vector2f& vel,
		const sf::FloatRect& target,
		sf::Vector2f& outNormal);
};