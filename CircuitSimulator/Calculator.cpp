#include <armadillo>
#include "Calculator.h"
#include "GridNode.h"
#include "Node.h"

using namespace arma;

bool Calculator::calculate(spot_vec spots, std::vector<Component*> components) {
    bool isCompleteCircuit = true;
    std::vector<GridSpot*> populatedSpots;

    // Only worry about spots that have components attached to them.
    // Keep track of whether or not the circuit is complete.
    // Condition for completeness:
    //      Every spots must have either 0 or (2 or more) components.
    //      A spot with only one component means that the component
    //      is "dangling", ie is only attached to a component on one
    //      end.
    for (int i = 0; i < spots.size(); i++) {
        for (int j = 0; j < spots[0].size(); j++) {
            if (spots[i][j]->components.size() > 0) {
                populatedSpots.push_back(spots[i][j]);
                if (spots[i][j]->components.size() == 1)
                    isCompleteCircuit = false;
            }
        }
    }

    // If the circuit is not complete, or the grid is completely empty,
    // set each component voltage and current to zero and return false.
    if (!isCompleteCircuit || populatedSpots.size() == 0) {
        for (int i = 0; i < components.size(); i++) {
            components[i]->setAmpsThrough(0.0);
            components[i]->setVoltageDrop(0.0);
        }

        return false;
    } else {
        std::vector<GridNode> gridNodes = getGridNodes(populatedSpots);
        std::vector<Node> nodes;

        // Convert GridNodes to Nodes

        // Initialize Nodes
        for (int i = 0; i < gridNodes.size(); i++) {
            Node node(i);
            gridNodes[i].id = i;
            nodes.push_back(node);
        }

        // Form Node connections
        for (int i = 0; i < nodes.size(); i++) {
            // List of gridNode components
            std::vector<Component*> components = gridNodes[i].getNonWireComponents();

            for (Component* component : components) {
                GridNode other;

                // Find the other gridNode this component is connected to
                for (GridNode gridNode : gridNodes) {
                    if (gridNode != gridNodes[i]) {
                        for (Component* otherComponent : gridNode.getNonWireComponents()) {
                            if (otherComponent == component) {
                                other = gridNode;
                            }
                        }
                    }
                }

                // Form connection between this Node and other Node
                nodes[i].addConnection(
                    &nodes[other.id],
                    component->getValue(),
                    component->getType() == &ComponentTypes::RESISTOR ? ValueType::OHM : ValueType::VOLT
                );
            }
        }

        for (Node node : nodes) {
            node.print(std::cout);
        }

        return true;
    }
}

std::vector<GridNode> Calculator::getGridNodes(std::vector<GridSpot*> spots) {
    // Holds all circuit nodes. A node is a section of the circuit that
    // shares a common voltage. In other words, the node at a point is
    // everything connected to that point via a wire.
    std::vector<GridNode> nodes;

    std::vector<GridSpot*>::iterator it = spots.begin();

    // Loop through populated spots and assign them to a Node.
    while (it != spots.end()) {
        bool seen = false;

        // Make sure this specific spot is not a part of any
        // other nodes. If it is, remove it from populatedSpots
        // and move to the next spot.
        if (nodes.size() > 0) {
            for (int i = 0; i < nodes.size(); i++) {
                for (int j = 0; j < nodes[i].spots.size(); j++) {
                    if (nodes[i].spots[j] == *it) {
                        seen = true;

                        // Grab next spot and set it as the iterator.
                        it = spots.erase(it);
                    }
                }
            }
        }

        if (!seen) {
            GridNode node;
            node.spots.push_back(*it);

            // Get Node from circuit recursively
            getGridNodeFromSpot(*it, node);

            nodes.push_back(node);
            ++it;
        }
    }

    return nodes;
}

GridNode Calculator::getGridNodeFromSpot(GridSpot* spot, GridNode& node) {
    std::vector<GridSpot*> spots = getNeighboorSpots(spot);

    for (int i = 0; i < spots.size(); i++) {
        GridSpot* nSpot = spots[i];
        bool seen = false;

        for (int j = 0; j < node.spots.size() && !seen; j++) {
            if (nSpot == node.spots[j])
                seen = true;
        }

        if (!seen) {
            node.spots.push_back(nSpot);

            getGridNodeFromSpot(nSpot, node);
        } 
    }

    return node;
}

std::vector<GridSpot*> Calculator::getNeighboorSpots(GridSpot* spot) {
    std::vector<GridSpot*> neighboors;

    for (int i = 0; i < spot->components.size(); i++) {
        Component* comp = spot->components[i];

        if (comp->getType() == &ComponentTypes::WIRE)
            neighboors.push_back(comp->getOther(spot));
    }

    return neighboors;
}