#include <iostream>
#include "Grid.h"
#include "Config.h"

Grid::Grid(int width, int height, int xOffset, int yOffset) {
	this->width = width;
	this->height = height;
	this->xOffset = xOffset;
	this->yOffset = yOffset;

	// Initialize spots, making sure they're centered in the board
	int leftPadding = (int) floor((width % SPOT_SPACING) / 2) + xOffset;
	int topPadding = (int) floor((height % SPOT_SPACING) / 2) + yOffset;

	int currX = leftPadding,
		currY = topPadding;

	bool shouldLoop = true;
	int numSpots = (width / SPOT_SPACING) * (height / SPOT_SPACING + 1) + 1;

	while (shouldLoop) {
		spots.push_back(new Spot(currX, currY));

		currX += SPOT_SPACING;

		if (currX > width) {
			currX = leftPadding;
			currY += SPOT_SPACING;
		}

		if (currY > height) {
			shouldLoop = false;
		}
	}
}

Grid::~Grid() {
	for (int i = 0; i < components.size(); i++)
		delete components.at(i);
	for (int i = 0; i < spots.size(); i++)
		delete spots.at(i);

	components.clear();
	spots.clear();
}

bool Grid::getNearestSpot(sf::Vector2i mousePos, Spot** spot) {
	int x = mousePos.x;
	int y = mousePos.y;

	if (x < xOffset || y < yOffset || x > width || y > height)
		return false;

	using std::cout;
	using std::endl;

	int leftPadding = (int) floor((width % SPOT_SPACING) / 2) + xOffset;
	int topPadding = (int) floor((height % SPOT_SPACING) / 2) + yOffset;

	int xIndex = (int) round(double(x - leftPadding) / double(SPOT_SPACING));
	int yIndex = (int) round(double(y - topPadding) / double(SPOT_SPACING));

	int xColumns = (int) ceil((width - leftPadding) / SPOT_SPACING) + 1;

	*spot = spots.at((yIndex * xColumns) + xIndex);
	return true;
}

void Grid::addComponent(Component* component) {
	components.push_back(component);
}