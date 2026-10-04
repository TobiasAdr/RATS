#include "../include/SpriteHandler.h"

SpriteHandler::SpriteHandler() {}

sf::IntRect SpriteHandler::getSpriteIndex(CharacterState state) {
  

	if (state == CharacterState::Idle) return sf::IntRect({ 22, 22 }, { 32, 32 });
	
	else if(state == CharacterState::RunningUP) return sf::IntRect({ 22, 202 }, { 32, 32 });
	
	else if(state == CharacterState::RunningDown) return sf::IntRect({ 22,18}, { 32, 32 });
	
	else if(state == CharacterState::RunningLeft) return sf::IntRect({ 22, 78 }, { 32, 32 });
	
	else if(state == CharacterState::RunningRight) return sf::IntRect({ 22, 138 }, { 32, 32 });
	
	else if(state == CharacterState::RunningUpLeft) return sf::IntRect({ 22, 44 }, { 32, 32 });
	
	else if(state == CharacterState::RunningUpRight) return sf::IntRect({ 22, 66 }, { 32, 32 });
	
	else if(state == CharacterState::RunningDownLeft) return sf::IntRect({ 22, 44 }, { 32, 32 });
	
	else if(state == CharacterState::RunningDownRight) return sf::IntRect({ 22, 66 }, { 32, 32 });

	return sf::IntRect({ 22, 22 }, { 32, 32 });
}