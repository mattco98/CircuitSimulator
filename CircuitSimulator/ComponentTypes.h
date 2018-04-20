#pragma once

#include <string>
#include "ComponentType.h"

namespace ComponentTypes {
    extern const int NUM_TYPES;
    extern const ComponentType WIRE;
    extern const ComponentType RESISTOR;
    extern const ComponentType VSRC;

    const ComponentType* getType(int i);
}