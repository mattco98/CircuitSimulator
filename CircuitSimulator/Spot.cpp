#include "Spot.h"

Spot::Spot(int x, int y) {
	this->x = x;
	this->y = y;
}

Spot::Spot(sf::Vector2i v) {
    x = v.x;
    y = v.y;
}

size_t Spot::getComponentNumber() {
	return components.size();
}

sf::Vector2f Spot::getVector() {
	return sf::Vector2f(x, y);
}

void Spot::addComponent(Component* component) {
	components.push_back(component);
}