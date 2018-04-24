/**
    Author:             Matthew Olsson
    File Title:         Calculator.h
    File Description:   Declares methods to calculate and set the voltage drop
                        and current through each component.
    Due Date:           4/25/2018
    Date Created:       4/10/2018
    Date Last Modified: 4/23/2018
*/

#pragma once

#include <vector>        // vector class
#include <utility>       // pair class
#include "Component.h"
#include "GridSpot.h"
#include "GridNode.h"
#include "Node.h"

typedef std::vector<std::vector<GridSpot*>> spot_vec;

namespace Calculator {
    /**
        Description:   Given a complete list of all components and GridSpots,
                       calculates the current through and voltage across each
                       component. 
        Return:        bool
        Precondition:  The GridSpot and Component vectors have been properly
                       initialized and contain all circuit GridSpots and
                       components, respectively.
        Postcondition: The Components will have their current and voltage
                       values modified. All other aspects of the components,
                       as well as the GridSpots, will remain unmodified.
    */
    bool calculate(spot_vec, std::vector<Component*>);

    /**
        Description:   Converts a vector of GridSpots into a vector of
                       GridNodes.
        Return:        vector<GridNode>
        Precondition:  The vector of GridSpots has been properly initialized
                       and populated.
        Postcondition: A vector of GridNodes will be returned. The vector of
                       GridSpots will not be modified.
    */
    std::vector<GridNode> getGridNodes(std::vector<GridSpot*>);

    /**
        Description:   Recursively creates a GridNode object given a GridSpot.
        Return:        GridNode
        Precondition:  The GridSpot has been properly initialized.
        Postcondition: The GridNode object passed in will be populated with
                       the spots that are connected to the GridSpot passed in
                       via wires (the first step in performing nodal analysis).
                       The GridSpot object will not be modified.
    */
    void getGridNodeFromSpot(GridSpot*, GridNode&);

    /**
        Description:   Given a GridSpot, returns all gridspots connected to
                       that spot via wires. Useful in creating and populating
                       GridNode objects.
        Return:        vector<GridSpot>
        Precondition:  The GridSpot passed in is valid and has been properly
                       initialized.
        Postcondition: A vector of neighboor GridSpots will be returned. The
                       argument passed in will not be modified.
    */
    std::vector<GridSpot*> getNodalNeighboorSpots(GridSpot*);

    /**
        Description:   Converts the "rough" GridNode objects into fully 
                       abstract Node objects.
        Return:        vector<Node>
        Precondition:  The vector passed in contains GridNodes that have been
                       properly initialized from the circuit.
        Postcondition: A vector of populated Node objects will be returned. The
                       argument will be modified in that the GridNodes will be
                       given IDs that correspond to their derived Node objects.
                       Can throw a runtime_error if a component's polarity has
                       been improperly set.
    */
    std::vector<Node> convertGridNodesToNodes(std::vector<GridNode>*);

    /**
        Description:   Reduces all nodes. This simply reduces parallel 
                       resistors to single resistors, and ensures there are no
                       voltage sources in parallel.
        Return:        vector<Node>
        Precondition:  The vector passed in contains Node objects that have
                       been properly initialized from the circuit.
        Postcondition: A vector of populated Node objects will be returned. The
                       argument will not be modified. If two voltage sources
                       are found to be in parallel, a runtime_error is thrown.
    */
    std::vector<Node> reduceNodes(std::vector<Node>);

    /**
        Description:   Reduces the connection between two nodes (ie: reduces
                       parallel resistors).
        Return:        void
        Precondition:  The Node objects passed in have been properly 
                       initialized and have valid connections.
        Postcondition: The Node arguments will have been modified if they share
                       parallel resistor connections. If two voltage sources
                       are found to be in parallel, a runtime_error is thrown.
                       A runtime_error is also thrown if the two nodes do not
                       share the same number of connections to each other.
    */
    void reduceNodeConnection(Node*, Node*);

    /**
        Description:   Given a list of all GridNodes and a specific Component,
                       gets the two GridNode objects that the Component is
                       connected to.
        Return:        pair<GridNode, GridNode>
        Precondition:  The GridNode vector contains all circuit GridNodes.
        Postcondition: A pair of GridNodes will be returns that correspond to
                       the GridNodes that surround the Component. The first
                       GridNode in the pair will always correspond to the
                       positive end of the Component. The arguments will not be
                       modified.
    */
    std::pair<GridNode, GridNode> getGridNodesFromComponent(std::vector<GridNode>, Component*);

    /**
        Description:   Given a vector of GridNodes and a specific GridSpot,
                       returns the GridNode that encapsulates the GridSpot.
        Return:        GridNode
        Precondition:  The GridNode vector contains all circuit GridNodes, and
                       the GridSpot is a valid spot on the Grid.
        Postcondition: The GridNode that encapsulates the GridSpot will be
                       returned. The arguments will not be modified.
    */
    GridNode getGridNodeFromSpot(std::vector<GridNode>, GridSpot*);

    /**
        Description:   
    */
    std::vector<Component*> getVoltageInputs(Component*, std::vector<Component*> = std::vector<Component*>(), int = 0);
}