#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Types.h"
#include <cstdint>
#include <iostream>
#include <vector>
#include <cmath>
#include <numbers>
#include <fstream>
#include <string>


class firePlaceOpenGL
{

public:

	int NUM_PARTICLES = 1024;

	firePlaceOpenGL(float originX, float originY);

	void update(float deltaTime);

	void render(const sf::View& view);

	void init();

	float getPosX() {

		return this->originX;
	}
	float getPosY() {

		return this->originY;
	}


	GLuint getTexture() { return firePlaceTexture; }

private:

	int W = 1600;
	int H = 1200;

	float originX = 0;
	float originY = 0;

	float elapsedTime = 0;

	GLuint firePlaceTexture;
	GLuint firePlaceSSBO;

	GLuint spawnFireProgram;
	GLuint simulateFLameProgram;

	GLuint renderProgram;
	GLuint particleVao;

	void step(float deltaTime);

};

