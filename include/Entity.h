#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

class Entity
{

public :
	virtual ~Entity() = default;
	virtual void update(float deltaTime) = 0;

};

