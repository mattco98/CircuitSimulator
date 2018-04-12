#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Component.h"

struct Spot {
	int x,
		y;

	// TODO: vector of tuples to track polarity?
	std::vector<Component*> components;

	Spot() = default;
	Spot(int x, int y);
    Spot(sf::Vector2i v);

	size_t getComponentNumber();
	sf::Vector2f getVector();
	void addComponent(Component* component);

	//operator sf::Vertex() const {
	//	return sf::Vertex(sf::Vector2f(x, y));
	//}
};