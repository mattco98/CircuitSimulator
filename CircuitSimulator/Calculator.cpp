#include "Calculator.h"

Calculator::Calculator(std::vector<Component*>* components, std::vector<GridSpot*>* spots) {
	this->components = components;

	// Get populated spots
	for (auto it = spots->begin(); it != spots->end(); ++it) {
		if ((*it)->getComponentNumber() > 0) {
			spots->push_back(*it);
		}
	}
}

Calculator::~Calculator() {
	delete components;
}

void calculate() {

}