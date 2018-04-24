/**
    Author:             Matthew Olsson
    File Title:         Calculator.cpp
    File Description:   Implements methods to calculate and set the voltage
                        drop and current through each component.
    Due Date:           4/25/2018
    Date Created:       4/10/2018
    Date Last Modified: 4/23/2018
*/

#include <armadillo>      // Lightweight linear algebra library
#include <stdexcept>      // runtime_error
#include "Calculator.h"

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
            components[i]->currentThrough = 0.0;
            components[i]->voltageDrop = 0.0;
        }
    } else {

        // Get Nodes from the circuit
        std::vector<GridNode> gridNodes = getGridNodes(populatedSpots);
        std::vector<Node> nodes = convertGridNodesToNodes(&gridNodes);
        std::vector<Node> reducedNodes = reduceNodes(nodes);

        // Begin the process of populating a matrix to solve for the 
        // node voltages

        arma::mat kcl(nodes.size(), nodes.size());
        arma::mat coeff(nodes.size(), 1);

        kcl.fill(0.0);
        coeff.fill(0.0);

        // In order to properly perform nodal analysis, one of the nodes must
        // be grounded. The node that gets grounded is completely arbitrary,
        // so here it is always the first node (node 0). The only side effect
        // is that some nodes may have negative voltage, but it is trivial to
        // "normalize" the voltages after they are calculated.
        const int GROUND = 0;

        // There are exactly "nodes.size()" number of equations. Since some equations
        // come from doing KCL at nodes and some come from voltage source relations,
        // the current row of the matrix that is being populated must be tracked,
        // especially after this initial loop has completed.
        int row = 0;

        // Every voltage source has two nodes attached to it, however only one
        // KCL equation can be derived from these two. To avoid populating the 
        // matrix with duplicate roww, the nodes that get considered as a 
        // consequence of considering the current node must be tracked and
        // excluded from the following process
        std::vector<Node*> excludedNodes;

        // Loop through the nodes and get every possible KCL equation
        for (Node node : reducedNodes) {
            if (node.id == GROUND) {
                // The voltage at the ground is 0
                kcl(row, GROUND) = 1;

                // Normally, when looking at a non-GROUND node, the nodes are examined
                // and excludedNodes is properly populated if the program runs across
                // any voltage sources. That is done here.
                for (auto connection : node.connections) {
                    if (std::get<2>(connection) == Unit::VOLT) {
                        excludedNodes.push_back(std::get<0>(connection));
                    }
                }

                row++;
            } else {
                bool consider = true;
                for (Node* excludedNode : excludedNodes) {
                    if (excludedNode->id == node.id)
                        consider = false;
                }

                if (consider) {
                    for (KCLTerm term : node.getTerms(nullptr, excludedNodes)) {
                        // The term.res (aka resistance of the resistor) corresponds to a 
                        // particular node. By the nature of the way KCL equations are
                        // derived, we need to keep track of the inverses of these 
                        // resistances. The sums of the inverses of the resistances is
                        // what the matrix will be populated with. Negative polarity 
                        // nodes will have their inverse sum negated.
                        kcl(row, term.pos) += 1.0 / term.res * (term.out ? -1.0 : 1.0);
                        kcl(row, term.neg) += -1.0 / term.res * (term.out ? -1.0 : 1.0);
                    }

                    row++;
                }
            }
        }

        // Loop through the nodes again, this time looking specifically for voltage
        // sources. Each voltage source provides a direct voltage relationship 
        // between its two nodes, and they are added to the matrices here. The
        // excluded nodes vector is re-used.
        excludedNodes.clear();

        for (Node node : reducedNodes) {
            bool consider = true;
            for (Node* excludedNode : excludedNodes) {
                if (excludedNode->id == node.id)
                    consider = false;
            }

            if (consider) {
                for (auto connection : node.connections) {
                    if (std::get<2>(connection) == Unit::VOLT) {
                        int pos,
                            neg;

                        if (std::get<3>(connection) == Polarity::POSITIVE) {
                            pos = node.id;
                            neg = std::get<0>(connection)->id;
                        } else {
                            pos = std::get<0>(connection)->id;
                            neg = node.id;
                        }

                        // In a direct voltage node equation, the relationship
                        // looks something along the lines of NODE1 - NODE2 = X,
                        // where NODE1 is the positive voltage terminal, NODE2
                        // is the negative voltage terminal, and X is the value
                        // of the voltage source. In a matrix, this means that
                        // the positive node has a weight of 1, the negative node
                        // a weight of -1, and an additional weight in the
                        // coefficient matrix.
                        kcl(row, pos) = 1.0;
                        kcl(row, neg) = -1.0;
                        coeff(row) = std::get<1>(connection);

                        // Exclude the other node connected to this voltage source
                        // to avoid duplicate matrix entries.
                        excludedNodes.push_back(std::get<0>(connection));

                        row++;
                    }
                }
            }
        }

        // Multiply the inverse kcl matrix by the coeff matrix to get the
        // solution voltages.
        arma::mat solution = kcl.i() * coeff;

        // Because our grounding point was arbitrary, some voltage may be negative.
        // This is easily fixed by subtracting the lowest value from all solution
        // matrix entries
        double min = solution.min();

        // Subtract min value from each element with a simple lambda function.
        solution.for_each([min](arma::mat::elem_type& val) { val -= min; });

        // Set the node voltage from the solution matrix. Because the matrices were
        // specifically ordered to match the node ids, the values in the first 
        // column correspond to the first node, the second column the second node,
        // etc.
        for (int i = 0; i < solution.n_rows; i++) {
            nodes[i].voltage = solution.at(i, 0);
        }

        // Start the process of turning node voltage into component voltages and currents.
        // On first pass, the current and voltage of every resistor is calculated.
        for (Component* component : components) {
            if (component->type == &RESISTOR) {
                std::pair<GridNode, GridNode> compGridNodes = getGridNodesFromComponent(gridNodes, component);
                std::pair<Node, Node> compNodes;

                for (Node tNode : nodes) {
                    if (tNode.id == compGridNodes.first.id)
                        compNodes.first = tNode;
                    else if (tNode.id == compGridNodes.second.id)
                        compNodes.second = tNode;
                }

                // compNodes.first will always be the positive node, and 
                // compNodes.second the negative
                double voltage = compNodes.first.voltage - compNodes.second.voltage;
                double current = voltage / component->value;

                component->voltageDrop = voltage;
                component->currentThrough = current;
            }
        }

        // The voltage sources are handled second, as, in order to find the current through
        // them, the current through all connected resistors must be known.
        for (Component* component : components) {
            if (component->type == &VSRC) {
                // Voltage drop across a vsrc is just its value
                component->voltageDrop = component->value;

                std::vector<Component*> resistors = getVoltageInputs(component);

                double currentSum = 0;

                // Sum up current inputs into the voltage source
                for (Component* resistor : resistors)
                    currentSum += resistor->currentThrough;

                component->currentThrough = currentSum;
            }
        }
    }

    return isCompleteCircuit && populatedSpots.size() != 0;
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

void Calculator::getGridNodeFromSpot(GridSpot* spot, GridNode& node) {
    // Get all spots within the node connected to this spot
    std::vector<GridSpot*> spots = getNodalNeighboorSpots(spot);

    // Adds spots to the node if they aren't in it already
    for (int i = 0; i < spots.size(); i++) {
        GridSpot* spot = spots[i];
        bool seen = false;

        for (int j = 0; j < node.spots.size() && !seen; j++) {
            if (spot == node.spots[j])
                seen = true;
        }

        if (!seen) {
            node.spots.push_back(spot);

            // Get connected spots recursively
            getGridNodeFromSpot(spot, node);
        } 
    }
}

std::vector<GridSpot*> Calculator::getNodalNeighboorSpots(GridSpot* spot) {
    std::vector<GridSpot*> neighboors;

    // Find all spots connected to this spot via wire components
    for (int i = 0; i < spot->components.size(); i++) {
        Component* comp = spot->components[i];

        if (comp->type == &WIRE)
            neighboors.push_back(comp->getOther(spot));
    }

    return neighboors;
}

std::vector<Node> Calculator::convertGridNodesToNodes(std::vector<GridNode>* gridNodes) {
    std::vector<Node> nodes;

    // Initialize Nodes
    for (int i = 0; i < gridNodes->size(); i++) {
        Node node(i);
        gridNodes->at(i).id = i;
        nodes.push_back(node);
    }

    // Form Node connections
    for (int i = 0; i < nodes.size(); i++) {
        // List of gridNode components
        std::vector<Component*> components = gridNodes->at(i).getNonWireComponents();

        for (Component* component : components) {
            GridNode other;

            // Find the other gridNode this component is connected to
            for (GridNode gridNode : *gridNodes) {
                if (gridNode != gridNodes->at(i)) {
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
                if (component->positive == spot) {
                    pol = Polarity::NEGATIVE;
                } else if (component->negative == spot) {
                    pol = Polarity::POSITIVE;
                }
            }

            if (pol == Polarity::UNKNOWN) {
                throw std::runtime_error("Unable to determine component polarity");
            }

            // Form connection between this Node and other Node
            nodes[i].addConnection(
                &nodes[other.id],
                component->value,
                component->type == &RESISTOR ? Unit::OHM : Unit::VOLT,
                pol
            );
        }
    }

    return nodes;
}

std::vector<Node> Calculator::reduceNodes(std::vector<Node> nodes) {
    std::vector<Node> reducedNodes = nodes;

    // Call reduceNodeConnection for all possible combinations of Nodes
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
    // Gets all connections from each node to the other
    auto node1Tuples = node1->getConnectionsToNode(node2);
    auto node2Tuples = node2->getConnectionsToNode(node1);

    if (node1Tuples.size() != node2Tuples.size())
        throw std::runtime_error("Nodes do not share an equal amount of connections");

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
            if (std::get<2>(t1) == Unit::OHM) {
                eqOhms += 1.0 / std::get<1>(t1);

                // Remove resistor connections. Will be replaced later with
                // single equivalent connection
                node1->removeConnection(t1);

                // Search for matching connection from node2 to node1
                bool found = false;
                for (auto t2 : node2Tuples) {
                    if (std::get<0>(t2)->id == node1->id &&
                        std::get<1>(t2) == std::get<1>(t1) &&
                        std::get<2>(t2) == std::get<2>(t1) &&
                        std::get<3>(t2) == !std::get<3>(t1) && !found) {
                        node2->removeConnection(t2);
                        found = true;
                    }
                }

                // Regather connections node to account for the connection
                // that was just (possibly) removed
                // TODO: If statement
                node1Tuples = node1->getConnectionsToNode(node2);
                node2Tuples = node2->getConnectionsToNode(node1);
            } else if (std::get<2>(t1) == Unit::VOLT) {
                numVolts++;
                i++;
            }
        }

        if (numVolts > 1)
            throw std::runtime_error("Voltage sources cannot be in parallel "
                                     "with each other");

        // Add equivalent resistor connection
        if (!(fabs(eqOhms) <= 1e-5)) {
            eqOhms = 1.0 / eqOhms;

            // Resistor polarity is arbitrary, however it is important that 
            // the polarities be opposite.
            node1->addConnection(node2, eqOhms, Unit::OHM, Polarity::POSITIVE);
            node2->addConnection(node1, eqOhms, Unit::OHM, Polarity::NEGATIVE);
        }
    }
}

std::pair<GridNode, GridNode> Calculator::getGridNodesFromComponent(std::vector<GridNode> gridNodes, Component* component) {
    return std::make_pair(
        getGridNodeFromSpot(gridNodes, component->positive),
        getGridNodeFromSpot(gridNodes, component->negative)
    );;
}

GridNode Calculator::getGridNodeFromSpot(std::vector<GridNode> gridNodes, GridSpot* spot) {
    GridNode node;

    // Find the GridNode that contains the spot
    for (GridNode gridNode : gridNodes) {
        for (GridSpot* gridSpot : gridNode.spots) {
            if (gridSpot == spot) {
                node = gridNode;
            }
        }
    }

    return node;
}

std::vector<Component*> Calculator::getVoltageInputs(Component* component, std::vector<Component*> resistors, int depth) {
    // Avoid stack overflows due to recursion
    if (depth > 10)
        throw std::runtime_error("Calculator::getVoltageInputs: Recursed past 10");

    // Get all connections component and merge into one vector
    std::vector<Component*> posComps = component->positive->components,
                            negComps = component->negative->components,
                            connectedComponents;
    connectedComponents.reserve(posComps.size() + negComps.size());
    connectedComponents.insert(connectedComponents.begin(), posComps.begin(), posComps.end());
    connectedComponents.insert(connectedComponents.begin(), negComps.begin(), negComps.end());

    // Loop through the components
    for (Component* comp : connectedComponents) {
        if (comp->type == &RESISTOR && comp->currentThrough > 0.0 &&
            // The component is a resistor and it has a positive current input.
            // This contributes positively to the current that goes into the
            // voltage source, so it gets added.
            (comp->negative == component->negative || comp->negative == component->positive)) {
            bool added = false;

            for (Component* resistor : resistors) {
                if (comp == resistor)
                    added = true;
            }

            if (!added)
                resistors.push_back(comp);
        } else if (comp->type == &VSRC && comp != component) {
            // Examine the voltage inputs for any neighbooring voltage sources.
            std::vector<Component*> newComps = getVoltageInputs(comp, resistors, ++depth);
            resistors.insert(resistors.end(), newComps.begin(), newComps.end());
        }
    }

    return resistors;
}