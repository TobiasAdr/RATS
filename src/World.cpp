#include "../include/World.h"
#include <sstream>

World::World() {}

void World::initWorld()
{

    std::ifstream file("resources/tileMap.txt");

    for (int row = 0; row < this->MAP_H; row++)
    {

        for (int col = 0; col < this->MAP_W; col++)
        {

            file >> tileMap[row][col];
        }
    }

    sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));

    for (int row = 0; row < MAP_H; row++)
    {
        for (int col = 0; col < MAP_W; col++)
        {
            
            switch (tileMap[row][col])
            {
            case 0:
                tile.setFillColor(sf::Color(100, 180, 100));
                break; 
            case 1:
                tile.setFillColor(sf::Color(180, 140, 100));
                break; 
            case 2:
                tile.setFillColor(sf::Color(60, 120, 200));
                break; 
            case 3:
                tile.setFillColor(sf::Color(30, 90, 30));
                break; 
            }

            tile.setPosition(sf::Vector2f(col * TILE_SIZE, row * TILE_SIZE));

            tiles.push_back(tile);
        }
    }
}

std::vector<std::vector<int>> World::loadMap(const std::string &path)
{

    std::vector<std::vector<int>> map;
    std::ifstream file(path);
    std::string line;

    while (std::getline(file, line))
    {
        std::vector<int> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ','))
            if (!cell.empty())
                row.push_back(std::stoi(cell));

        if (!row.empty())
            map.push_back(row);
    }

    return map;
}
