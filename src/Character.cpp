#include "../include/Character.h"

Character::Character(int health, sf::Vector2f size, float gravity, float jumpStrength, float speed, sf::Vector2f velocity, sf::Color color)
{

    this->health = health;
    this->body.setSize(size);
    this->gravity = gravity;
    this->jumpStrength = jumpStrength;
    this->speed = speed;
    this->velocity = velocity;
    this->canJump = false;
    this->body.setFillColor(color);
}

void Character::applyGravity(float deltaTime)
{

    if (!this->canJump)
    {
        this->velocity.y += this->gravity * deltaTime;
    }
};
void Character::moveLeft()
{

    this->velocity.x = -this->speed;
};
void Character::moveRight()
{

    this->velocity.x = this->speed;
};
void Character::moveUp()
{

    this->velocity.y = -this->speed;
};
void Character::moveDown()
{

    this->velocity.y = this->speed;
};
void Character::attack() {};
void Character::duck() {};
void Character::jump()
{

    this->velocity.y = -sqrtf(2.0f * this->gravity * this->jumpStrength);
};
void Character::move(float deltaTime)
{

    this->body.move(this->velocity * deltaTime);
};
void Character::bounceBack()
{
    this->velocity.y = -sqrtf(2.0f * this->gravity * this->jumpStrength);
    this->velocity.x -= 50;
};

void Character::setPosition(sf::Vector2f position) { this->body.setPosition(position); }
void Character::setCanJump(bool canJump) { this->canJump = canJump; }
void Character::setVelocityY(float velocityY) { this->velocity.y = velocityY; }
void Character::setVelocityX(float velocityX) { this->velocity.x = velocityX; }
void Character::removeHP(int hp)
{

    this->health = this->health - hp;
}

sf::Vector2f Character::getSize() { return this->body.getSize(); }
sf::RectangleShape Character::getBody() { return body; }
sf::Vector2f Character::getPosition() { return this->body.getPosition(); }
sf::Vector2f Character::getVelocity() { return this->velocity; }
bool Character::getCanJump() { return this->canJump; }
int Character::getHP()
{
    return this->health;
}
void Character::update(float deltaTime)
{

    this->applyGravity(deltaTime);
    this->move(deltaTime);
};
void Character::setTexture(sf::Texture &texture)
{

    this->body.setTexture(&texture);
}
void Character::setTextureRect(sf::IntRect rect)
{

    this->body.setTextureRect(rect);
}
void Character::setSize(sf::Vector2f size)
{

    this->body.setSize(size);
}
void Character::setScale(sf::Vector2f scale)
{

    this->body.setScale(scale);
}
