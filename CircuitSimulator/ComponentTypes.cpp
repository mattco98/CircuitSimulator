#include "ComponentTypes.h"

const int ComponentTypes::NUM_TYPES = 3;
const ComponentType ComponentTypes::WIRE(0, "Wire");
const ComponentType ComponentTypes::RESISTOR(1, "Resistor");
const ComponentType ComponentTypes::VSRC(2, "Voltage Source");

const ComponentType* ComponentTypes::getType(int i) {
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