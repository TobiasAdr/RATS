#include "Types.h"
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "../include/Platform.h"
#include <iostream>

class LightManager
{
private:

public:
	bool collision(const std::vector<std::vector<std::vector<Platform*>>>& grid, sf::Vector2f Pos);


	void castRay(const std::vector<std::vector<std::vector<Platform*>>>& grid, sf::RenderWindow& window,Ray ray, int stepSize);

};

