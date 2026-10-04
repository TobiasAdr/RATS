#pragma once
#include "Character.h"
#include "SpriteHandler.h"
#include <iostream>
#include <optional>
class Enemy : public Character
{

protected:
	sf::Texture IdleTexture;
	sf::Texture RunTexture;

	SpriteHandler spriteHandler;

	CharacterState enemyState = CharacterState::Idle;

	std::optional<sf::Sprite> enemySprite;
	void updateTexture();

	void setDirection(); 
	sf::Vector2f playerPosition;

	sf::IntRect getSpriteIndex(CharacterState state);

public:
	Enemy();

	void loadTexture(std::string idlePath, std::string runPath);

	
	void update(float deltaTime) override;
	void setPlayerPosition(sf::Vector2f playerPosition);

	std::optional<sf::Sprite> getEnemySprite();
};