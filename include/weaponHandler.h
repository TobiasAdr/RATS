#include "Types.h"
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class WeaponHandler
{
private:
	bool Staff;
	bool Sword;
	bool Bow;
	bool Gun;

public:
	WeaponHandler();
	~WeaponHandler();
	void setStaff(bool hasStaff);
	void setSword(bool hasSword);
	void setBow(bool hasBow);
	void setGun(bool hasGun);
	bool hasStaff() const;
	bool hasSword() const;
	bool hasBow() const;
	bool hasGun() const;

	Fireball setFireball(sf::Vector2f playerPosition, sf::Vector2f attackDirection);

	

};

