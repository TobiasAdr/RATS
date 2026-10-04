#include "../include/LightManager.h"

bool LightManager::collision(const std::vector<std::vector<std::vector<Platform *>>> &grid, sf::Vector2f Pos)
{

	int gX = static_cast<int>(Pos.x / 100.f);
	int gY = static_cast<int>(Pos.y / 100.f);

	int gridWidth = static_cast<int>(grid.size());
	int gridHeight = static_cast<int>(grid[0].size());

	if (gX < 0 || gX >= gridWidth || gY < 0 || gY >= gridHeight)
	{
		return false;
	}

	const auto &cell = grid[gX][gY];
	if (cell.empty())
		return false;

	for (Platform *platform : cell)
	{
		sf::FloatRect platformBounds = platform->getShape().getGlobalBounds();
		if (platformBounds.contains(Pos))
		{
			return true;
		}
	}

	return false;
}

void LightManager::castRay(const std::vector<std::vector<std::vector<Platform *>>> &grid, sf::RenderWindow &window, Ray ray, int stepSize)
{

	float X = ray.origin.x;
	float Y = ray.origin.y;
	float dirX = ray.direction.x;
	float dirY = ray.direction.y;

	for (int i = 0; i < 20; i++)
	{

		if (!collision(grid, sf::Vector2f(X, Y)))
		{
			X += dirX * stepSize;
			Y += dirY * stepSize;
		}
		else
			break;
	}

	sf::Vertex line[2];
	line[0].position = sf::Vector2f(ray.origin.x, ray.origin.y);
	line[0].color = sf::Color::Red;
	line[1].position = sf::Vector2f(X, Y);
	line[1].color = sf::Color::Red;
}
