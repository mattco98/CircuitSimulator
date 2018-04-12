#include "ComponentTypes.h"

const ComponentType ComponentTypes::WIRE = ComponentType(0, "Wire");
const ComponentType ComponentTypes::RESISTOR = ComponentType(1, "Resistor");
const ComponentType ComponentTypes::VSRC = ComponentType(2, "Voltage Source");

const ComponentType& ComponentTypes::getType(int index) {
	switch (index) {
		case 0: 
			return ComponentTypes::WIRE; 
			break;
		case 1: 
			return ComponentTypes::RESISTOR; 
			break;
		case 2:
			return ComponentTypes::VSRC;
			break;
	}
}