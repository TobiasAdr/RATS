#include "../include/Enemy.h"

Enemy::Enemy() : Character(100, sf::Vector2f(10.f, 10.f), 981.f, 500.f, 20.f, sf::Vector2f(0.f, 0.f), sf::Color::Red) {}

void Enemy::setDirection()
{

	float y = this->getPosition().y;
	float x = this->getPosition().x;

	float playerx = this->playerPosition.x;
	float playery = this->playerPosition.y;

	if (playerx < x)
	{

		this->enemyState = CharacterState::RunningLeft;
		this->moveLeft();
	}
	else if (playerx > x)
	{

		this->moveRight();
	}
	else
	{

		this->enemyState = CharacterState::RunningRight;
		this->velocity.x = 0.f;
	}

	if (playery < y)
	{

		this->enemyState = CharacterState::RunningUP;
		this->moveUp();
	}
	else if (playery > y)
	{
		this->enemyState = CharacterState::RunningDown;
		this->moveDown();
	}
	else
	{
		this->enemyState = CharacterState::Idle;
		this->velocity.x = 0.0f;
	}
};

void Enemy::update(float deltaTime)
{

	this->setDirection();
	this->updateTexture();
	this->move(deltaTime);
};

void Enemy::setPlayerPosition(sf::Vector2f playerPosition)
{
	this->playerPosition = playerPosition;
}

void Enemy::loadTexture(std::string idlePath, std::string runPath)
{

	this->IdleTexture.loadFromFile(idlePath);
	this->RunTexture.loadFromFile(runPath);

	this->enemySprite.emplace(this->IdleTexture);

	this->enemySprite->setTextureRect(sf::IntRect({19, 20}, {32, 32}));
	this->enemySprite->setScale(sf::Vector2f(0.5f, 0.5f));
}

void Enemy::updateTexture()
{

	if (this->enemyState != CharacterState::Idle)
	{
		this->enemySprite->setTexture(this->RunTexture);
	}
	else
	{
		this->enemySprite->setTexture(this->IdleTexture);
	}

	sf::IntRect spriteRect = this->getSpriteIndex(this->enemyState);

	this->enemySprite->setTextureRect(spriteRect);
	this->enemySprite->setScale(sf::Vector2f(0.5f, 0.5f));

	this->enemySprite->setPosition(this->getPosition());
}

std::optional<sf::Sprite> Enemy::getEnemySprite()
{

	return this->enemySprite;
};

sf::IntRect Enemy::getSpriteIndex(CharacterState state)
{

	if (state == CharacterState::Idle)
		return sf::IntRect({19, 20}, {32, 32});

	else if (state == CharacterState::RunningUP)
		return sf::IntRect({22, 202}, {32, 32});

	else if (state == CharacterState::RunningDown)
		return sf::IntRect({22, 18}, {32, 32});

	else if (state == CharacterState::RunningLeft)
		return sf::IntRect({22, 78}, {32, 32});

	else if (state == CharacterState::RunningRight)
		return sf::IntRect({22, 138}, {32, 32});

	else if (state == CharacterState::RunningUpLeft)
		return sf::IntRect({22, 44}, {32, 32});

	else if (state == CharacterState::RunningUpRight)
		return sf::IntRect({22, 66}, {32, 32});

	else if (state == CharacterState::RunningDownLeft)
		return sf::IntRect({22, 44}, {32, 32});

	else if (state == CharacterState::RunningDownRight)
		return sf::IntRect({22, 66}, {32, 32});

	return sf::IntRect({22, 22}, {32, 32});
}