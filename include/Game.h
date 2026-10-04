#pragma once
#include <SFML/System.hpp>
#include "EntityManager.h"
#include "GameRenderer.h"
#include "InputHandler.h"

class Game {
private:
    EntityManager entityManager;
    GameRenderer gameRenderer;
    InputHandler inputHandler;
    sf::Clock clock;

    void mouseInput();
    void keyboardInput();
    void checkAndHandleInputs();
    void processEvents();
    void gameLoop();
    void update();
    void render();

public:
    Game();
    void startGame();
};