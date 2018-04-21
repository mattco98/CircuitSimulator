#include <armadillo>
#include <stdexcept>
#include "Calculator.h"
#include "GridNode.h"

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
        std::vector<Node> nodes = convertGridNodesToNodes(gridNodes);
        std::vector<Node> reducedNodes = reduceNodes(nodes);

        for (Node node : reducedNodes) {
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

std::vector<Node> Calculator::convertGridNodesToNodes(std::vector<GridNode> gridNodes) {
    std::vector<Node> nodes;

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

            Polarity pol = Polarity::UNKNOWN;

            // Determine polarity
            for (GridSpot* spot : other.spots) {
                if (component->getPositive() == spot) {
                    pol = Polarity::NEGATIVE;
                } else if (component->getNegative() == spot) {
                    pol = Polarity::POSITIVE;
                }
            }

            if (pol == Polarity::UNKNOWN) {
                throw std::runtime_error("Unable to determine component polarity");
            }

            // Form connection between this Node and other Node
            nodes[i].addConnection(
                &nodes[other.id],
                component->getValue(),
                component->getType() == &ComponentTypes::RESISTOR ? ValueType::OHM : ValueType::VOLT,
                pol
            );
        }
    }

    return nodes;
}

std::vector<Node> Calculator::reduceNodes(std::vector<Node> nodes) {
    std::vector<Node> reducedNodes = nodes;

    for (int i = 0; i < nodes.size(); i++) {
        for (int j = 0; j < nodes.size(); j++) {
            if (i != j) {
                reduceNodeConnection(&reducedNodes[i], &reducedNodes[j]);
            }
        }
    }

    return reducedNodes;
}

void Calculator::reduceNodeConnection(Node* node1, Node* node2) {
    auto node1Tuples = node1->getConnectionsToNode(node2);
    auto node2Tuples = node2->getConnectionsToNode(node1);

    if (node1Tuples.size() == 0 && node2Tuples.size() == 0) {
        return;
    }

    if (node1Tuples.size() != node2Tuples.size()) {
        throw std::runtime_error("Nodes do not share an equal amount of connections");
    }

    // Reduce connections
    if (node1Tuples.size() > 1) {
        // Voltages sources cannot be in parallel. If there is more than
        // one voltage source connection between these nodes, throw an
        // error.
        int numVolts = 0;
        double eqOhms = 0.0;

        int i = 0;
        while (node1Tuples.size() > 0 && i < node1Tuples.size()) {
            auto t1 = node1Tuples[i];
            if (std::get<2>(t1) == ValueType::OHM) {
                eqOhms += 1.0 / std::get<1>(t1);

                // Remove resistor connections. Will be replaced later with
                // single equivalent connection
                node1->removeConnection(t1);

                // Search for matching connection from node2 to node1
                for (auto t2 : node2Tuples) {
                    if (std::get<0>(t2)->id == node1->id &&
                        std::get<1>(t2) == std::get<1>(t1) &&
                        std::get<2>(t2) == std::get<2>(t1) && 
                        std::get<3>(t2) == !std::get<3>(t1)) {
                        node2->removeConnection(t2);
                        break;
                    }
                }

                node1Tuples = node1->getConnectionsToNode(node2);
                node2Tuples = node2->getConnectionsToNode(node1);
            } else if (std::get<2>(t1) == ValueType::VOLT) {
                numVolts++;
                i++;
            }
        }

        if (numVolts > 1) {
            throw std::runtime_error("Voltage sources cannot be in parallel "
                                     "with each other");
        }
        
        // Add equivalent resistor connection
        if (!(fabs(eqOhms) <= 1e-5)) {
            eqOhms = 1.0 / eqOhms;

            // Resistor polarity is arbitrary, however it is important that 
            // the polarities be opposite.
            node1->addConnection(node2, eqOhms, ValueType::OHM, Polarity::POSITIVE);
            node2->addConnection(node1, eqOhms, ValueType::OHM, Polarity::NEGATIVE);
        }
    }
}