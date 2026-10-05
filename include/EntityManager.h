#include "OpenGL/OpenGLElementSimulation.h"
#include "OpenGL/OpenGLWaterSimulation.h"
#include "OpenGL/OpenGLFireSimulation.h"
#include "OpenGL/OpenGLBouncingRods.h"
#include "OpenGL/EnemyManagerOpenGL.h"
#include "OpenGL/TileRendererOpenGL.h"
#pragma once
#include "Player.h"
#include "OpenGL/firePlaceOpenGL.h"
#include <SFML/Graphics.hpp>
#include "World.h"
#include <memory>
#include <optional>
#include <vector>

class EntityManager
{

private:
	float spawnTimer = 0.f;
	float spawnInterval = 0.2f;

	Player player;
	sf::Texture playerIdleTexture;
	Weapon weapon = Weapon::Water;

	std::unique_ptr<OpenGLElementSimulation> sandSimulator;
	std::unique_ptr<OpenGLWaterSimulation> waterSimulator;
	std::unique_ptr<OpenGLFireSimulation> fireSimulator;
	std::unique_ptr<OpenGLBouncingRods> rodsSimulator;

	TileRendererOpenGL tileRenderer;
	firePlaceOpenGL firePlace = firePlaceOpenGL(875.0f, 825.0f);

	EnemyManagerOpenGL enemyManager;

	std::vector<sf::CircleShape> colliders;

	const int TILE_W = 18;
	const int TILE_H = 18;

	World world;
	std::vector<std::vector<int>> layerObjects;

	std::optional<sf::Sprite> playerSprite;

	void initPlayer(int windowWidth, int windowHeight);
	void updatePlayer(float deltaTime);

	void initEnemies();
	void updateEnemies(float deltaTime);

	void spawnEnemies(float deltaTime);

	void setupMap();
	void loadTerrain();

public:
	EntityManager();
	Player &getPlayer();
	std::optional<sf::Sprite> getPlayerSprite();
	sf::Vector2f getPlayerPos();

	Weapon getWeapon() { return this->weapon; };

	void setWeapon(Weapon weapon) { this->weapon = weapon; };

	void update(float deltaTime);
	void init(int width, int height);

	OpenGLElementSimulation &getSandSimulation() { return *this->sandSimulator; }
	OpenGLWaterSimulation &getWaterSimulation() { return *this->waterSimulator; }
	OpenGLFireSimulation &getFireSimulation() { return *this->fireSimulator; }
	OpenGLBouncingRods &getRodsSimulation() { return *this->rodsSimulator; }

	EnemyManagerOpenGL &getEnemyManager() { return this->enemyManager; }
	TileRendererOpenGL &getTileRenderer() { return this->tileRenderer; }
	firePlaceOpenGL &getFirePlace() { return this->firePlace; }
};
