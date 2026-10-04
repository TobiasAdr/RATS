#pragma once
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class SpriteHandler
{

public:
    SpriteHandler();

    sf::IntRect getSpriteIndex(CharacterState state);

};

