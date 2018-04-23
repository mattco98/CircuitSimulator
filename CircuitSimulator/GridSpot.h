#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Component.h"

struct GridNode;

struct GridSpot {
    const int x,
              y;

    std::vector<Component*> components;

    inline GridSpot(int x, int y) : x(x), y(y) { };
    inline GridSpot(sf::Vector2i v) : x(v.x), y(v.y) { };

    inline sf::Vector2f getVector() const { 
        return sf::Vector2f{ float(x), float(y) }; 
    }
};