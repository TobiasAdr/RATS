#include "../include/GameRenderer.h"
#include <string>

GameRenderer::GameRenderer(EntityManager& entityManager)
    : glRenderer(window), entityManager(entityManager)
{
    this->setupWindow();
    font.openFromFile("resources/PressStart2P-Regular.ttf");
}

void GameRenderer::setupWindow() {

    window.create(sf::VideoMode::getDesktopMode(), "Dodgeball", sf::State::Fullscreen);

    const sf::Vector2u windowSize = window.getSize();
    this->camera.setSize(sf::Vector2f(windowSize.x / 4.f, windowSize.y / 4.f));

    window.setActive(true);

    glewExperimental = GL_TRUE;
    glewInit();
}

void GameRenderer::render() {

    if (this->entityManager.getPlayer().getHP() > 0) {
        window.clear();

        this->camera.setCenter(this->entityManager.getPlayerPos());
        window.setView(camera);

        window.resetGLStates();
        window.pushGLStates();
        glRenderer.render(this->entityManager.getWaterSimulation(), this->entityManager.getFireSimulation(), this->entityManager.getRodsSimulation(), this->entityManager.getEnemyManager(), this->entityManager.getTileRenderer(), this->entityManager.getFirePlace());
        window.popGLStates();
        window.resetGLStates();

        this->window.draw(*this->entityManager.getPlayerSprite());

        window.setView(window.getDefaultView());
        
        sf::Text scoreText(font, "Score: " + std::to_string(this->entityManager.getEnemyManager().getScore()), 24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(sf::Vector2f(10.f, 50.f));
        window.draw(scoreText);

        window.draw(scoreText);

        float maxHP = 100.f;
        float currentHP = this->entityManager.getPlayer().getHP();
        float barWidth = 100.f;
        float barHeight = 20.f;

        
        sf::RectangleShape hpBackground(sf::Vector2f(barWidth, barHeight));
        hpBackground.setFillColor(sf::Color(100, 0, 0));
        hpBackground.setPosition(sf::Vector2f(10.f, 10.f));

        
        sf::RectangleShape hpBar(sf::Vector2f(barWidth * (currentHP / maxHP), barHeight));
        hpBar.setFillColor(sf::Color(0, 200, 0));
        hpBar.setPosition(sf::Vector2f(10.f, 10.f));

        window.draw(hpBackground);
        window.draw(hpBar);

        window.setView(camera);

        window.display();
    }
    else {
        window.clear(sf::Color::Black);
        window.setView(window.getDefaultView());

        sf::Text gameOverText(font, "GAME OVER", 64);
        gameOverText.setFillColor(sf::Color::Red);
        sf::FloatRect bounds = gameOverText.getLocalBounds();
        gameOverText.setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
        gameOverText.setPosition(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f));

        window.draw(gameOverText);
        window.display();


    }

}

sf::RenderWindow& GameRenderer::getWindow() {

    return this->window;

}

void GameRenderer::initRenderer() {
    glRenderer.init(
        this->entityManager.getWaterSimulation(),
        this->entityManager.getFireSimulation(),
        this->entityManager.getRodsSimulation(),
        this->entityManager.getEnemyManager(),
        this->entityManager.getTileRenderer(),
        this->entityManager.getFirePlace()
    );
}