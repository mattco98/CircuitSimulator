#include "GridSpot.h"

GridSpot::GridSpot(int x_, int y_){
    x = x_;
    y = y_;
    node = nullptr;
};

GridSpot::GridSpot(sf::Vector2i v) {
    x = v.x;
    y = v.y;
    node = nullptr;
};

sf::Vector2f GridSpot::getVector() const {
    return sf::Vector2f{ float(x), float(y) };
};