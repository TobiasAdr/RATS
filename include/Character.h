#include "Types.h"
#pragma once
#include "Entity.h"

class Character : public Entity
{
protected:
	
	int health;
	
	
	sf::RectangleShape body;
	sf::Color color;

	
	sf::Vector3f position;
	sf::Vector3f size;

	
	sf::Vector2f velocity;
	float gravity;
	float jumpStrength;
	float speed;
	bool canJump;

	
	virtual void applyGravity(float deltaTime);
	virtual void move(float deltaTime);
	virtual void attack();
	virtual void duck();

public:
	Character(int health, sf::Vector2f size, float gravity, float jumpStrength, float speed, sf::Vector2f velocity, sf::Color color);

	


	
	virtual void jump();
	virtual void moveLeft();
	virtual void moveRight();
	virtual void moveUp();
	virtual void moveDown();
	virtual void bounceBack();
	
	
	sf::Vector2f getSize();
	sf::RectangleShape getBody();
	sf::Vector2f getPosition();
	sf::Vector2f getVelocity();
	bool getCanJump();
	int getHP();
	
	
	void setPosition(sf::Vector2f position);
	void setCanJump(bool canJump);
	void setVelocity(sf::Vector2f velocity) {

		this->velocity = velocity;
	}
	void setVelocityY(float velocityY);
	void setVelocityX(float velocityX);
	void removeHP(int hp);
	void setTexture(sf::Texture& texture);
	void setTextureRect(sf::IntRect rect);
	void setSize(sf::Vector2f size);
	void setScale(sf::Vector2f scale);
	
	virtual void update(float deltaTime) override;


};

