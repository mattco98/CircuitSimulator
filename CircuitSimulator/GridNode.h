#pragma once

#include <vector>
#include "GridSpot.h"

struct GridNode {
    int id;
    std::vector<GridSpot*> spots;

    std::vector<Component*> getNonWireComponents() const;

    inline bool operator ==(GridNode other) const { return id == other.id; }
    inline bool operator !=(GridNode other) const { return id != other.id; }
};