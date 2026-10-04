#pragma once
#include "OpenGL/OpenGLRenderer.h"
#include <SFML/Graphics.hpp>
#include "EntityManager.h"

class GameRenderer
{
private:
    OpenGLRenderer glRenderer;
    EntityManager& entityManager;
    sf::RenderWindow window;
    sf::View camera;
    sf::Font font;

    void setupWindow();

public:
    explicit GameRenderer(EntityManager& entityManager);
    sf::RenderWindow& getWindow();

    void initRenderer();
    sf::View& getCamera() { return camera; }
    void render();
};
