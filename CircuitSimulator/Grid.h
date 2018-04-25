/**
    Author:             Matthew Olsson
    File Title:         Grid.h
    File Description:   Declares the Grid class. This class is responsible for
                        maintaining and storing the components and GridSpots.
                        All Components and GridSpots are stored as pointers
                        so they can be passed around and manipulated by other
                        classes.
    Due Date:           4/25/2018
    Date Created:       3/24/2018
    Date Last Modified: 4/23/2018
*/

#pragma once

#include <vector>             // vector class, .push_back(), .at(), .clear(),
                              // .erase(), .begin(), .end()
#include <SFML/Graphics.hpp>  // Objects: Vector2i/f, .x, .y
#include "Component.h"
#include "GridSpot.h"

class Grid { 
	private:
        int width,
            height,
            numSpotsHorz,
            numSpotsVert,
            paddingHorz,
            paddingVert;

		std::vector<Component*> components;
		std::vector<std::vector<GridSpot*>> spots;

	public:
        /**
            Description:   Initializes a Grid object.
            Return:        None
            Precondition:  None
            Postcondition: A Grid object will be initialized. width and height
                           will be set to the size specified in Config.cpp,
                           and the spots array will be filled with enough spots
                           to fill the grid dimensions.
        */
        Grid() = default;

        /**
            Description:   Initializes a Grid object with the provided width
                           and height.
            Return:        None
            Precondition:  None
            Postcondition: A Grid object will be initialized. width and height
                           will be set to the size specified in Config.cpp,
                           and the spots array will be filled with enough spots
                           to fill the grid dimensions.
        */
		Grid(int, int);

        /**
            Description:   Returns the components vector.
            Return:        vector<Component*>
            Precondition:  This object exists.
            Postcondition: The components array will be returned. This object
                           will not be modified.
        */
		std::vector<Component*> getComponents() const;

        /**
            Description:   Returns the spots vector
            Return:        vector<vector<GridSpot*>>
            Precondition:  This object exists.
            Postcondition: The spots array will be returned. This object will
                           not be modified.
        */
		std::vector<std::vector<GridSpot*>> getSpots() const;

        /**
            Description:   Gets the nearest spot to the mouse cursor. 
            Return:        bool
            Precondition:  This object exists, and the GridSpot passed in is a 
                           valid GridSpot pointer reference.
            Postcondition: If the mouse is within the grid area, the spot
                           argument will be set to the closest spot, and the
                           function will return true. Else, spot will be 
                           unmodified. Neither this object nor mousePos will be
                           modified.
        */
        bool getNearestSpot(sf::Vector2i, GridSpot*&) const;

        /**
            Description:   Gets the component under the mouse cursor. Searches
                           for components within 14 pixels of the cursor.
            Return:        bool
            Precondition:  This object exists, and the Component passed in is a
                           valid Component pointer reference.
            Postcondition: If a component is within 14 pixels of the mouse
                           cursor, the component pointer will be set to that
                           component and the function will return true. Else,
                           the function will return false, and the component
                           pointer will not be modified. Neither this object
                           nor the vector will be modified.
        */
        bool getComponentUnderPosition(sf::Vector2i, Component*&) const;

        /**
            Description:   Clears the components vector. Additionally, loops 
                           through the spots vector and removes all component
                           references.
            Return:        void
            Precondition:  This object exists.
            Postcondition: The components vector will be cleared, and all
                           GridSpots will have any component references 
                           cleared.
        */
        void clearComponents();

        /**
            Description:   Adds a component to the components vector.
            Return:        void
            Precondition:  This object exists, and the component pointer passed
                           in has been properly initialized.
            Postcondition: A component will be added to the component vector.
                           The component will not be modified.
        */
		void addComponent(Component*);

        /**
            Description:   Removes the component from the components vector
                           as well as from both the spots it was connected to.
            Return:        void
            Precondition:  This object exists.
            Postcondition: The component will have been removed from both
                           the components array and from the GridSpots it was
                           connected to.
        */
        void removeComponent(Component*);
};
