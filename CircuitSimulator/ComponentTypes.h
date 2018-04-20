#pragma once

#include <string>

struct ComponentType {
    int value;
    std::string name;

    ComponentType(int v, std::string n) : value(v), name(n) {}

    inline int getValue() const { return value; }
    inline std::string getName() const { return name; }
};

namespace ComponentTypes {
    const int NUM_TYPES = 3;
    const ComponentType WIRE(0, "Wire");
    const ComponentType RESISTOR(1, "Resistor");
    const ComponentType VSRC(2, "Voltage Source");

    inline const ComponentType* getType(int i) {
        switch (i) {
            case 0:
                return &WIRE;
                break;
            case 1:
                return &RESISTOR;
                break;
            case 2:
                return &VSRC;
                break;
        }
        return nullptr;
    };
}