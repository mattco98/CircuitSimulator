#pragma once

#include <string>
#include <SFML/Window/Keyboard.hpp>

struct ComponentType {
    std::string name;

    ComponentType(std::string);

    std::string getName() const;
};

extern const ComponentType WIRE;
extern const ComponentType RESISTOR;
extern const ComponentType VSRC;

void shift(const ComponentType*&, sf::Keyboard::Key);