#pragma once

#include <vector>
#include "Component.h"
#include "GridSpot.h"

class Calculator {
	private:
		GridSpot* ground;
		std::vector<Component*>* components;
		std::vector<GridSpot*> spots;

	public:
		Calculator(std::vector<Component*>* components, std::vector<GridSpot*>* spots);
		~Calculator();

		void calculate();
};

