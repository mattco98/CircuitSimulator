#pragma once

#include <string>
#include "ComponentTypes.h"

struct GridSpot;

struct Component {
    const ComponentType* const type;
    
    double value,
           voltageDrop,
           currentThrough;

    GridSpot* positive;
    GridSpot* negative;

    inline Component(const ComponentType* type) : type(type), value(5.0) { }

    inline GridSpot* getOther(GridSpot* s) const {
        return s == positive ? negative : (s == negative ? positive : nullptr);
    }
};