#include "ComponentTypes.h"

ComponentType::ComponentType(std::string n) {
    name = n;
}

std::string ComponentType::getName() const {
    return name;
}

const ComponentType WIRE("Wire");
const ComponentType RESISTOR("Resistor");
const ComponentType VSRC("Voltage Source");

void shift(const ComponentType*& type, sf::Keyboard::Key key) {
    switch (key) {
        case sf::Keyboard::Up:
            if (type == &WIRE) type = &VSRC;
            else if (type == &RESISTOR) type = &WIRE;
            else if (type == &VSRC) type = &RESISTOR;
            break;
        case sf::Keyboard::Down:
            if (type == &WIRE) type = &RESISTOR;
            else if (type == &RESISTOR) type = &VSRC;
            else if (type == &VSRC) type = &WIRE;
            break;
    }
}