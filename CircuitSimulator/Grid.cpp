#include <iostream>
#include <cmath>
#include "Grid.h"
#include "Config.h"

Grid::Grid() {
	this->width = SCREEN_WIDTH - GRID_LEFT_OFFSET - GRID_RIGHT_OFFSET;
	this->height = SCREEN_HEIGHT - GRID_TOP_OFFSET - GRID_BOTTOM_OFFSET;

    int numSpotsHorz = width / SPOT_SPACING;
    int numSpotsVert = height / SPOT_SPACING;

    int paddingHorz = (width - (numSpotsHorz - 1) * SPOT_SPACING) / 2;
    int paddingVert = (height - (numSpotsVert - 1) * SPOT_SPACING) / 2;

    int currX = GRID_LEFT_OFFSET + paddingHorz;
    int currY = GRID_TOP_OFFSET + paddingVert;

    spots = std::vector< std::vector<GridSpot*> >(numSpotsVert, std::vector<GridSpot*>(numSpotsHorz));

    for (int i = 0; i < numSpotsVert; i++) {
        std::vector<GridSpot*> row = std::vector<GridSpot*>(numSpotsHorz);
        for (int j = 0; j < numSpotsHorz; j++) {
            row[j] = new GridSpot(currX, currY);
            currX += SPOT_SPACING;
        }
        spots[i] = row;
        currX = GRID_LEFT_OFFSET + paddingHorz;
        currY += SPOT_SPACING;
    }
}

bool Grid::getNearestSpot(sf::Vector2i mousePos, GridSpot** spot) const {
    int numSpotsHorz = width / SPOT_SPACING;
    int numSpotsVert = height / SPOT_SPACING;

    int paddingHorz = (width - (numSpotsHorz - 1) * SPOT_SPACING) / 2;
    int paddingVert = (height - (numSpotsVert - 1) * SPOT_SPACING) / 2;

    int x = mousePos.x - GRID_LEFT_OFFSET - paddingHorz;
    int y = mousePos.y - GRID_TOP_OFFSET - paddingVert;

    int xIndex = (int) round(double(x) / double(SPOT_SPACING));
    int yIndex = (int) round(double(y) / double(SPOT_SPACING));

    if (xIndex < 0 || yIndex < 0 || xIndex > numSpotsHorz - 1 || yIndex > numSpotsVert - 1)
        return false;

    *spot = spots[yIndex][xIndex];
    return true;
}

void Grid::addComponent(Component* component) {
	components.push_back(component);
}

bool Grid::getComponentUnderPosition(sf::Vector2i pos, Component* &nearestComp) const {
    double x,
        y,
        dx,
        dy;
    GridSpot* spot1;
    GridSpot* spot2;
    Component* component;

    for (int i = 0; i < components.size(); i++) {
        component = components.at(i);

        spot1 = component->getPositive();
        spot2 = component->getNegative();

        x = spot1->x;
        y = spot1->y;

        dx = (spot2->x - x) / 100.0;
        dy = (spot2->y - y) / 100.0;

        for (int j = 0; j < 100; j++) {
            if (fabs(x - double(pos.x)) < 10.0 && fabs(y - double(pos.y)) < 10.0) {
                nearestComp = component;
                return true;
            }

            x += dx;
            y += dy;
        }
    }

    return false;
}