#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Component.h"

struct GridSpot {
	int x,
		y;

	// TODO: vector of tuples to track polarity?
	std::vector<Component*> components;

	GridSpot() = default;
	GridSpot(int x, int y);
    GridSpot(sf::Vector2i v);

	size_t getComponentNumber();
	sf::Vector2f getVector();
	void addComponent(Component* component);
};