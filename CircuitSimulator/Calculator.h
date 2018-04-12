#pragma once

#include <vector>
#include "Component.h"
#include "Spot.h"

class Calculator {
	private:
		Spot* ground;
		std::vector<Component*>* components;
		std::vector<Spot*> spots;

	public:
		Calculator(std::vector<Component*>* components, std::vector<Spot*>* spots);
		~Calculator();

		void calculate();
};

