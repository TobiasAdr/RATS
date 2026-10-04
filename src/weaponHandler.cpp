#include "../include/weaponHandler.h"

WeaponHandler::WeaponHandler() : Staff(false), Sword(false), Bow(false), Gun(false) {}
WeaponHandler::~WeaponHandler() {}
void WeaponHandler::setStaff(bool hasStaff) { Staff = hasStaff; }
void WeaponHandler::setSword(bool hasSword) { Sword = hasSword; }
void WeaponHandler::setBow(bool hasBow) { Bow = hasBow; }
void WeaponHandler::setGun(bool hasGun) { Gun = hasGun; }
bool WeaponHandler::hasStaff() const { return Staff; }
bool WeaponHandler::hasSword() const { return Sword; }
bool WeaponHandler::hasBow() const { return Bow; }
bool WeaponHandler::hasGun() const { return Gun; }

Fireball WeaponHandler::setFireball(sf::Vector2f playerPosition, sf::Vector2f attackDirection) {

	Fireball fireball;
	fireball.position = playerPosition;
	fireball.direction = attackDirection;
	fireball.shape.setFillColor(sf::Color::Red);
	fireball.shape.setPosition(playerPosition);
	fireball.shape.setRadius(5.f);
	return fireball;

};