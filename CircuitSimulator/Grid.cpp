/**
    Author:             Matthew Olsson
    File Title:         Grid.h
    File Description:   Implements the Grid class. This class is responsible 
                        for maintaining and storing the components and 
                        GridSpots. All Components and GridSpots are stored as 
                        pointers so they can be passed around and manipulated 
                        by other classes.
    Due Date:           4/25/2018
    Date Created:       3/24/2018
    Date Last Modified: 4/23/2018
*/

#include "Grid.h"
#include "Config.h"

Grid::Grid() {
	width = SCREEN_WIDTH - GRID_LEFT_OFFSET - GRID_RIGHT_OFFSET;
	height = SCREEN_HEIGHT - GRID_TOP_OFFSET - GRID_BOTTOM_OFFSET;

    // Calculate total number of spots in each direction
    numSpotsHorz = width / SPOT_SPACING;
    numSpotsVert = height / SPOT_SPACING;

    // Calculate the padding requires to center the GridSpots in the middle
    // of the grid
    paddingHorz = (width - (numSpotsHorz - 1) * SPOT_SPACING) / 2;
    paddingVert = (height - (numSpotsVert - 1) * SPOT_SPACING) / 2;

    // Keep track of x/y position for iterating
    int currX = GRID_LEFT_OFFSET + paddingHorz;
    int currY = GRID_TOP_OFFSET + paddingVert;

    // Initialize spots vector
    spots = std::vector<std::vector<GridSpot*>>(numSpotsVert, std::vector<GridSpot*>(numSpotsHorz));

    // Iterate and populate the spots vector with GridSpot pointers
    for (int i = 0; i < numSpotsVert; i++, currY += SPOT_SPACING) {
        std::vector<GridSpot*> row = std::vector<GridSpot*>(numSpotsHorz);
        for (int j = 0; j < numSpotsHorz; j++, currX += SPOT_SPACING) 
            row[j] = new GridSpot(currX, currY);
        spots[i] = row;
        currX = GRID_LEFT_OFFSET + paddingHorz;
    }
}

std::vector<Component*> Grid::getComponents() const {
    return components;
};

std::vector<std::vector<GridSpot*>> Grid::getSpots() const {
    return spots;
};

bool Grid::getNearestSpot(sf::Vector2i mousePos, GridSpot*& spot) const {
    // Localize the x and y coordinates within the grid
    int x = mousePos.x - GRID_LEFT_OFFSET - paddingHorz;
    int y = mousePos.y - GRID_TOP_OFFSET - paddingVert;

    // Calculate the x and y index within the spots array of the closest spot
    // to the mouse positions
    int xIndex = (int) round(double(x) / double(SPOT_SPACING));
    int yIndex = (int) round(double(y) / double(SPOT_SPACING));
    
    // Ensure indices are not out of the grid bounds
    bool invalid = xIndex < 0 || yIndex < 0 || xIndex > numSpotsHorz - 1 || yIndex > numSpotsVert - 1;
    if (!invalid)
        spot = spots[yIndex][xIndex];

    return !invalid;
}

bool Grid::getComponentUnderPosition(sf::Vector2i pos, Component*& nearestComp) const {
    double x,
           y,
           dx,
           dy;
    GridSpot* spot1;
    GridSpot* spot2;
    Component* component;
    bool found = false;

    // Iterate through all components and check if the mouse position is within
    // 14 pixels of that component
    for (int i = 0; i < components.size() && !found; i++) {
        component = components.at(i);

        spot1 = component->positive;
        spot2 = component->negative;

        x = double(spot1->x);
        y = double(spot1->y);

        dx = (spot2->x - x) / 40.0;
        dy = (spot2->y - y) / 10.0;

        // Loop along the line that the component forms
        for (int j = 0; j < 40 && !found; j++) {
            if (fabs(x - double(pos.x)) < 14.0 && fabs(y - double(pos.y)) < 14.0) {
                // The mouse is within 14 pixels of this component
                nearestComp = component;
                found = true;
            }

            x += dx;
            y += dy;
        }
    }

    return found;
}

void Grid::clearComponents() {
    components.clear();

    // Clear all component references from the GridSpots
    for (std::vector<GridSpot*> row : spots) {
        for (GridSpot* spot : row) {
            spot->components.clear();
        }
    }
}

void Grid::addComponent(Component* component) {
	components.push_back(component);
}

void Grid::removeComponent(Component* component) {
    std::vector<Component*>::iterator it = components.begin();
    bool deleted;

    // Removed the component from both spots
    for (GridSpot* spot : { component->positive, component->negative }) {
        deleted = false;
        std::vector<Component*>::iterator it = spot->components.begin();
        while (!deleted && it != spot->components.end()) {
            if (*it == component) {
                spot->components.erase(it);
                deleted = true;
            } else {
                ++it;
            }
        }
    }

    // Remove the component from the components array
    deleted = false;
    while (!deleted && it != components.end()) {
        if (*it == component) {
            components.erase(it);
            deleted = true;
        } else {
            ++it;
        }
    }
}