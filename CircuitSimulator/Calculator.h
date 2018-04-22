#pragma once

#include <vector>
#include "Component.h"
#include "GridSpot.h"
#include "Node.h"

typedef std::vector< std::vector<GridSpot*> > spot_vec;

namespace Calculator {
    bool calculate(spot_vec, std::vector<Component*>);

    std::vector<GridNode> getGridNodes(std::vector<GridSpot*> spots);

    GridNode getGridNodeFromSpot(GridSpot*, GridNode&);

    std::vector<GridSpot*> getNeighboorSpots(GridSpot*);

    std::vector<Node> convertGridNodesToNodes(std::vector<GridNode>*);

    std::vector<Node> reduceNodes(std::vector<Node>);

    void reduceNodeConnection(Node*, Node*);

    std::vector<GridNode> getGridNodesFromComponent(std::vector<GridNode>, Component*);

    GridNode getGridNodeFromSpot(std::vector<GridNode>, GridSpot*);

    std::vector<Component*> getVoltageInputs(Component*, std::vector<Component*> = std::vector<Component*>());
}