#pragma once

#include <SFML/Graphics.hpp>

namespace GuiHelper {
    void drawLine(sf::RenderWindow&, sf::Vertex, sf::Vertex);
    void drawLine(sf::RenderWindow&, sf::Vector2f, sf::Vector2f, sf::Color = sf::Color::White);
    void drawRectangleHollow(sf::RenderWindow&, sf::Vector2f, sf::Vector2f, sf::Vector2f, sf::Vector2f, sf::Color color);
    sf::Color applyAlpha(sf::Color, double);
}
