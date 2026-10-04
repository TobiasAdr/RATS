#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include "../Types.h"
#include "SpriteRenderer.h"

class TileRendererOpenGL
{


public:

	TileRendererOpenGL();

	void init();
	void render(GLuint shaderProgram, GLuint windowProgram, const sf::View& view);

private:

	GLuint windowVao = 0;
	GLuint windowTexture = 0;  
	GLuint windowVbo = 0;
	GLuint windowEbo = 0;

	glm::mat4 viewProj;

	void initWindowMesh();     
	void drawWindowTile(GLuint shader, float x, float y);

	unsigned int vao, vbo, ebo;

	unsigned int wallVao, wallVbo, wallEbo;

	unsigned int wallNorthVao, wallNorthVbo, wallNorthEbo;

	GLuint floorTexture;
	GLuint mossTexture;

	static constexpr float TILE_W = 32.0f;
	static constexpr float TILE_H = 16.0f;

	void drawFloorTile(GLuint shaderProgram, float screenX, float screenY, float r, float g, float b);
	void drawMossFloor(GLuint shaderProgram, float screenX, float screenY, float r, float g, float b);

	void drawWallTile(GLuint shaderProgram, float screenX, float screenY, float r, float g, float b);

	void windowLight(GLuint shaderProgram, float screenX, float screenY, float r, float g, float b);


	void drawWindowRay(GLuint shaderProgram, float windowX, float windowY, float floorX, float floorY);
	void drawRays(GLuint shaderProgram, float windowX, float windowY,
		float floorX, float floorY);

};