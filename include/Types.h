#include <GL/glew.h>
#include <glm/glm.hpp>
#pragma once
#include <SFML/Graphics.hpp>

struct Ray {
	sf::Vector2f origin;
	sf::Vector2f direction;
};

struct Fireball {
	sf::Vector2f position;
	sf::Vector2f direction;
	sf::CircleShape shape;
	bool hitEnemy = false;
};

struct FirePlaceParticle {

	float posX, posY;
	float dirX, dirY;
	float life;

};

enum class Weapon {

	Fire,
	Water,
	Rod

};

enum class Cell : uint8_t { Empty, Sand, Water };

enum class WaterCell : uint8_t { Water, Empty};
 
struct GridParticle {
	Cell type;
	int life;
	bool sleeping = false;
	int idleLife = 0;

};

struct FireParticleCPU {
	float posX = 0, posY = 0;
	float prevX = 0, prevY = 0;
	float dirX = 0, dirY = 0;
	float startX = 0, startY = 0;
	int life = 0;
	int flying = 0;
};

struct FireParticle {

	int type;
	int life;
	float posX, posY;
	float startX, startY;
	float dirX, dirY;
	float prevX, prevY;
	int flying;

};

struct PurpleRod {
	float startX, startY;
	float prevStartX, prevStartY;
	float prevEndX, prevEndY;
	float endX, endY;
	float dirX, dirY;
	float distTravelled;
	float maxDist;
};

struct FirePixel {
	int posX, posY;
	float r, g, b, a;
};

struct EnemyOpenGL {
	float x, y;
	float hp;
	bool alive;
};

struct GPUEnemy {
	float x, y;
	float pad[2]; 
};


struct WaterParticle {

	int type;
	int life;
	float posX;
	float posY;
	float startX, startY;
	float dirX, dirY;

};

struct FlyingParticle {
	float x, y;       
	float vx, vy;     
	int life;
};

struct Chunk {
	bool active;
	bool sleeping;
};

struct SandRay {
	float x, y;          
	float vx, vy;        
	float life;          
	float spreadAngle;   
};

enum class CharacterState {
	Idle,
	RunningUP,
	RunningDown,
	RunningLeft,
	RunningRight,
	RunningUpLeft,
	RunningUpRight,
	RunningDownLeft,
	RunningDownRight
};

struct Particle {
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec4 color;
	float     life;        
	float     size;
	float     rotation;
	float     type;        
};

