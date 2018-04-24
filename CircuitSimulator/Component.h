/**
    Author:             Matthew Olsson
    File Title:         Component.h
    File Description:   Declares and implements the Component struct. This
                        struct represents an individual circuit component that
                        the user has placed on the grid. It stores information
                        about the component, depending on its ComponentType.
    Due Date:           4/25/2018
    Date Created:       3/25/2018
    Date Last Modified: 4/23/2018
*/

#pragma once

#include "ComponentTypes.h"

// Because GridSpot and Component have a circular dependency, GridSpot is
// forward declared rather than included to avoid redefinition.
struct GridSpot;

struct Component {
    const ComponentType* const type;
    
    double value,
           voltageDrop,
           currentThrough;

    GridSpot* positive;
    GridSpot* negative;

    /**
        Description:   Initializes a Component object with the specified type
                       and a default value of 5.0.
        Return:        None
        Precondition:  None
        Postcondition: A Component object will be initialize with the specified
                       type and a default value of 5.0.
    */
    inline Component(const ComponentType* type) : type(type), value(5.0) { }

    /**
        Description:   If this component is connected to the provided spot, 
                       return the other spot that this component is connected
                       to. Allows recursive traversal of a circuit.
        Return:        GridSpot*
        Precondition:  This object exists and the GridSpot pointer passed in is
                       valid.
        Postcondition: The other GridSpot* connected to this component will be
                       returned. This object will not be modified.
    */
    inline GridSpot* getOther(GridSpot* s) const {
        return s == positive ? negative : (s == negative ? positive : nullptr);
    }
};