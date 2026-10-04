#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Platform
{

private: 

	sf::RectangleShape shape; 
	sf::Vector2f position; 


public:

	
	Platform();
	

	
	void setPosition(sf::Vector2f position);
	void setSize(sf::Vector2f size);
	void setColor(sf::Color color);

	
	sf::RectangleShape getShape();
	sf::Vector2f getSize();
	sf::Vector2f getPosition();



};

