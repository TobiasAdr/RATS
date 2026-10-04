#include "../include/inputHandler.h"

InputHandler::InputHandler(EntityManager& entityManager)
    : entityManager(entityManager),  
    player(entityManager.getPlayer())
{
   
}

void InputHandler::gameKeyboardA() {
    player.moveLeft();
	player.setPlayerState(CharacterState::RunningLeft);
};
void InputHandler::gameKeyboardD() {
    player.moveRight();
	player.setPlayerState(CharacterState::RunningRight);
};
void InputHandler::gameKeyboardW() {
    player.moveUp();
	player.setPlayerState(CharacterState::RunningUP);
};
void InputHandler::gameKeyboardS() {
    player.moveDown();
	player.setPlayerState(CharacterState::RunningDown);
};

void InputHandler::gameKeyboardF() {

    Weapon currentWeapon = entityManager.getWeapon();

    if (currentWeapon == Weapon::Water) {

        entityManager.setWeapon(Weapon::Fire);

    }
    else if (currentWeapon == Weapon::Fire){
        
        entityManager.setWeapon(Weapon::Rod);

    }

    else{
    
        entityManager.setWeapon(Weapon::Water);
    }

}

void InputHandler::gameMouseInput(sf::Vector2i mousePos, sf::Vector2f mouseWorld)
{

		sf::Vector2f playerPos = player.getPosition();
		sf::Vector2f middlePlayer = playerPos + player.getSize() / 2.f;

        sf::Vector2f direction = mouseWorld - middlePlayer;
		sf::Vector2f normalizedDirection = direction / std::sqrtf(direction.x * direction.x + direction.y * direction.y);

        player.setAttackDirection(normalizedDirection);

		sf::Vector2f attackPosition = middlePlayer + normalizedDirection * player.getSwordLength();

		player.playerAttack(attackPosition);

}

void InputHandler::closeWindow(sf::RenderWindow& window) {

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        window.close();
    }

}

void InputHandler::processWindowEvents(sf::RenderWindow& window) {
    this->closeWindow(window);
}