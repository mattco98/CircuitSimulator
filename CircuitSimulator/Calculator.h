#pragma once

#include <iostream>
#include <vector>
#include "Component.h"
#include "GridSpot.h"

typedef std::vector< std::vector<GridSpot*> > spot_vec;

namespace Calculator {
    bool calculate(spot_vec, std::vector<Component*>);

    std::vector<GridNode> getGridNodes(std::vector<GridSpot*> spots);

    GridNode getGridNodeFromSpot(GridSpot*, GridNode&);

    std::vector<GridSpot*> getNeighboorSpots(GridSpot*);
}