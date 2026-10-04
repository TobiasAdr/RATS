#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <fstream>


class World
{

private:

	const int TILE_SIZE = 18;
	const int MAP_H = 32;
	const int MAP_W = 32;
	int tileMap[32][32]; 


	std::vector<sf::RectangleShape> tiles;

	
public: 

	World();						

	void initWorld();

	std::vector<sf::RectangleShape> getTiles() const { return tiles; }

	std::vector<std::vector<int>> loadMap(const std::string& path);

};

