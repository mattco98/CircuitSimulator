#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Component.h"

struct GridSpot {
	int x,
		y;

	// TODO: vector of tuples to track polarity?
	std::vector<Component*> components;

    GridSpot(int x, int y) : x(x), y(y) {};
    GridSpot(sf::Vector2i v) : x(v.x), y(v.y) {};

    inline sf::Vector2f getVector() const { return sf::Vector2f{ float(x), float(y) }; };
    inline std::vector<Component*> getComponents() const { return components; }
    inline void addComponent(Component* component) { components.push_back(component); };
};