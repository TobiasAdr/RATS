#include "../include/Player.h"
#include <iostream>

Player::~Player(){}

Player::Player():Character(1000, sf::Vector2f(16, 16), 981.f, 500.f, 50.f, sf::Vector2f(0.f,0.f),sf::Color::White)
{
    this->swordLength = 50.f;
}

void Player::update(float deltaTime){
    
   
    this->move(deltaTime);
 
};


void Player::playerJump() {
    
    
    if (this->canJump) {
        this->jump();
        this->canJump = false;
    }
}

void Player::playerAttack(sf::Vector2f attackPosition) {

	sf::RectangleShape attackRect;
	attackRect.setSize(sf::Vector2f(10.f, 10.f));
    attackRect.setFillColor(sf::Color::Blue);
	attackRect.setPosition(attackPosition);
    this->attackPosition = attackRect;

}

float Player::getSwordLength() const {
    return this->swordLength;
}

sf::Vector2f Player::getAttackDirection() const {

    return this->attackDirection;
}

void Player::setAttackDirection(sf::Vector2f direction) {
    
    this->attackDirection = direction;

}
