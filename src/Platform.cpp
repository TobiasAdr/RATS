#include "../include/Platform.h"


Platform::Platform() {};

void Platform::setPosition(sf::Vector2f position) {

	this->shape.setPosition(position);
}

void Platform::setSize(sf::Vector2f size) {
	this->shape.setSize(size);
}

void Platform::setColor(sf::Color color) {
	this->shape.setFillColor(color);
}

sf::RectangleShape Platform::getShape() {

	return this->shape;

};
sf::Vector2f Platform::getSize() {

	return this->shape.getSize();

};
sf::Vector2f Platform::getPosition() {
	return this->shape.getPosition();
};