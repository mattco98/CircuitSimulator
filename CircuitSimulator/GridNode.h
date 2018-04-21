#pragma once

#include <iostream>
#include <vector>
#include "GridSpot.h"

struct GridNode {
    int id;
    std::vector<GridSpot*> spots;

    std::vector<Component*> getNonWireComponents() const;

    inline bool operator ==(GridNode other) { return id == other.id; }
    inline bool operator !=(GridNode other) { return id != other.id; }

    // TODO: DEBUG
    void print(std::ostream&) const;
};