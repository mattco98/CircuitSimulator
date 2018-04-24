/**
    Author:             Matthew Olsson
    File Title:         GridNode.h
    File Description:   Declares the GridNode struct. This struct represents
                        a "rough node" (see Node.h). In other words, it is the
                        first step in the process of performing nodal analysis
                        on the circuit built by the user.
    Due Date:           4/25/2018
    Date Created:       4/10/2018
    Date Last Modified: 4/23/2018
*/

#pragma once

#include <vector>      // vector class, .push_back()
#include "GridSpot.h"

struct GridNode {
    int id;
    std::vector<GridSpot*> spots;

    /**
        Description:   Initializes a GridNode object with no set values.
        Return:        None
        Precondition:  None
        Postcondition: Returns an initialized GridNode object.
    */
    GridNode() = default;

    /**
        Description:   Returns all non-wire components in this node.
        Return:        vector<Component*>
        Precondition:  This object exists
        Postcondition: All non-wire components connected to this node are
                       returned. This object will not be modified.
    */
    std::vector<Component*> getNonWireComponents() const;

    /**
        Description:   Determines equality with another node based on the
                       node's ID.
        Return:        bool
        Precondition:  Both GridNode objects exist and have valid IDs.
        Postcondition: Returns true if the objects share the same ID, false
                       otherwise. This object will not be modified.
    */
    inline bool operator ==(GridNode other) const { return id == other.id; }

    /**
        Description:   Determines non-equality with another node based on the
                       node's ID.
        Return:        bool
        Precondition:  Both GridNode objects exist and have valid IDs.
        Postcondition: Returns false if the objects share the same ID, true
                       otherwise. This object will not be modified.
    */
    inline bool operator !=(GridNode other) const { return id != other.id; }
};