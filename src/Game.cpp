#include "../include/Game.h"

Game::Game()
    : entityManager(),
      gameRenderer(entityManager),
      inputHandler(entityManager)
{
}

void Game::mouseInput() {

    sf::RenderWindow& window = this->gameRenderer.getWindow();
    while (auto event = window.pollEvent()) {

        if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            if (keyEvent->scancode == sf::Keyboard::Scan::F) {  
                this->inputHandler.gameKeyboardF();
            }
        }

        if (event->is<sf::Event::MouseButtonPressed>()) {
            auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();

 
            if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left) {

                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePos, this->gameRenderer.getCamera());

                this->inputHandler.gameMouseInput(mousePos, mouseWorld);

                float dirX = this->entityManager.getPlayer().getAttackDirection().x;
                float dirY = this->entityManager.getPlayer().getAttackDirection().y;

                float playerPosX = this->entityManager.getPlayerPos().x;
                float playerPosY = this->entityManager.getPlayerPos().y;

                Weapon currentWeapon = entityManager.getWeapon();

                if (currentWeapon == Weapon::Water) {
                 
                    this->entityManager.getWaterSimulation().spawnWaterComputeShader(playerPosX, playerPosY, dirX, dirY);
                
                }
                
                else if (currentWeapon == Weapon::Fire) {
                
                    this->entityManager.getFireSimulation().spawnFire(playerPosX, playerPosY, dirX, dirY);
                
                }
                
                else if (currentWeapon == Weapon::Rod) {
                
                    this->entityManager.getRodsSimulation().spawnRods(playerPosX, playerPosY, dirX, dirY);
            
                }
            }
        }
    }

};

void Game::keyboardInput() {
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) this->inputHandler.gameKeyboardA();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) this->inputHandler.gameKeyboardD();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) this->inputHandler.gameKeyboardW();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) this->inputHandler.gameKeyboardS();

   
    
};

void Game::checkAndHandleInputs() {

    this->mouseInput();
    this->keyboardInput();

};

void Game::gameLoop() {
   
    while (this->gameRenderer.getWindow().isOpen())
    {

        this->update();
		this->render();
    
    }

};

void Game::startGame() {
    this->entityManager.init(
        this->gameRenderer.getWindow().getSize().x,
        this->gameRenderer.getWindow().getSize().y
    );

    this->gameRenderer.initRenderer(); 

    this->gameLoop();
}

void Game::processEvents() {
     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
     {
         this->gameRenderer.getWindow().close();
     }
}

void Game::update()
{

    this->processEvents();

    const float deltaTime = clock.restart().asSeconds();

    this->checkAndHandleInputs();
    
    this->entityManager.update(deltaTime);
    

}

void Game::render()
{
  
    this->gameRenderer.render();

}
