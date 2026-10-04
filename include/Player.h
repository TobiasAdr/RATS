#pragma once
#include "Character.h"

class Player :public Character
{

private:
	float swordLength;
	sf::RectangleShape attackPosition;
	sf::Vector2f attackDirection;
	bool isRunning;
	CharacterState playerState = CharacterState::Idle;

public:
	Player();
	~Player();
	
	
	void update(float deltaTime) override;
	void playerJump();
	void playerMoveUp();
	void playerAttack(sf::Vector2f attackPosition);

	
	void setAttackDirection(sf::Vector2f direction);
	void setSwordLength(float length) { this->swordLength = length; }
	void setRunning(bool running) { this->isRunning = running; }
	void setPlayerState(CharacterState state) { this->playerState = state; }
	
	
	float getSwordLength() const;
	sf::Vector2f getAttackPosition() const;
	sf::Vector2f getAttackDirection() const;
	bool running() const { return this->isRunning; }
	CharacterState getPlayerState() const { return this->playerState; }

	sf::RectangleShape getAttackRect() const { return this->attackPosition; }

	
	void shoot();
	void swordSlash();
	sf::CircleShape shootFireball(float deltaTime);
	void frostbolt();
	void flamewave(); 

};

