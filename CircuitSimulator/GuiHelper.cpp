#include "GuiHelper.h"


void GuiHelper::drawLine(sf::RenderWindow& window, sf::Vertex p1, sf::Vertex p2) {
    const sf::Vertex vertices[2] = { p1, p2 };
    window.draw(vertices, 2, sf::PrimitiveType::Lines);
}

void GuiHelper::drawLine(sf::RenderWindow& window, sf::Vector2f p1, sf::Vector2f p2, sf::Color color) {
    sf::Vertex v1(p1),
        v2(p2);

    v1.color = color;
    v2.color = color;

    GuiHelper::drawLine(window, v1, v2);
}

void GuiHelper::drawRectangleHollow(sf::RenderWindow& window, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4, sf::Color color) {
    GuiHelper::drawLine(window, p1, p2, color);
    GuiHelper::drawLine(window, p2, p3, color);
    GuiHelper::drawLine(window, p3, p4, color);
    GuiHelper::drawLine(window, p4, p1, color);
}