#pragma once
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include <stdlib.h>
#include "weaponHandler.h"
#include "EntityManager.h"
class InputHandler
{
private:
    Player& player;
    EntityManager& entityManager;
    WeaponHandler weaponHandler;

public:
   
    InputHandler(EntityManager& entityManager);
    
	

    void gameKeyboardA();
    void gameKeyboardD();
    void gameKeyboardW();
    void gameKeyboardS();
	void gameKeyboardF();

    void gameMouseInput(sf::Vector2i mousePos, sf::Vector2f mouseWorld);

	
    void closeWindow(sf::RenderWindow& window);
    void processWindowEvents(sf::RenderWindow& window);


};

