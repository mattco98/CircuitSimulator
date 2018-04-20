#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Component.h"

struct Node;

struct GridSpot {
	int x,
		y;

	// TODO: vector of tuples to track polarity?
	std::vector<Component*> components;
    Node* node;

    GridSpot(int, int);
    GridSpot(sf::Vector2i);

    sf::Vector2f getVector() const;
};