#pragma once

#include <string>

struct ComponentType {
    int value;
    std::string name;

    ComponentType(int value, std::string name) {
        this->value = value;
        this->name = name;
    };

    inline int getValue() const { return value; }
    inline std::string getName() const { return name; }
};

namespace ComponentTypes {
    const int NUM_TYPES = 3;
    const ComponentType WIRE(0, "Wire"),
                        RESISTOR(1, "Resistor"),
                        VSRC(2, "Voltage Source");

    inline const ComponentType* getType(int i) {
        switch (i) {
            case 0:
                return &ComponentTypes::WIRE;
                break;
            case 1:
                return &ComponentTypes::RESISTOR;
                break;
            case 2:
                return &ComponentTypes::VSRC;
                break;
        }
    };
}