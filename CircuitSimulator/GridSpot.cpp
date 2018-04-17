#include "GridSpot.h"
#include <iostream>

GridSpot::GridSpot(int x, int y) {
	this->x = x;
	this->y = y;
}

GridSpot::GridSpot(sf::Vector2i v) {
    x = v.x;
    y = v.y;
}

size_t GridSpot::getComponentNumber() {
	return components.size();
}

sf::Vector2f GridSpot::getVector() {
	return sf::Vector2f(float(x), float(y));
}

void GridSpot::addComponent(Component* component) {
	components.push_back(component);
}