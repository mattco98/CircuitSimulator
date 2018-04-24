/**
    Author:             Matthew Olsson
    File Title:         GridSpot.h
    File Description:   Declares the GridSpot struct. Represents a spot on the
                        grid. Holds a list of components connected to it so
                        the circuit can determine which components are
                        connected to other components.
    Due Date:           4/25/2018
    Date Created:       3/24/2018
    Date Last Modified: 4/23/2018
*/

#pragma once

#include <vector>                   // Vector class
#include <SFML/System/Vector2.hpp>  // sf::Vector2f
#include "Component.h"

struct GridSpot {
    const int x,
              y;

    std::vector<Component*> components;

    /**
        Description:   Initializes a Grid with the provided coordinates.
        Return:        None
        Precondition:  None
        Postcondition: Returns a Grid object with the provided coordinates.
    */
    inline GridSpot(int x, int y) : x(x), y(y) { };

    /**
        Description:   Returns the coordinates in as a float vector.
        Return:        sf::Vector2f
        Precondition:  This object exists.
        Postcondition: A vector of the coordinate is returned.
    */
    inline sf::Vector2f getVector() const { 
        return sf::Vector2f{ float(x), float(y) }; 
    }
};