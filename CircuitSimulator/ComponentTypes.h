#pragma once

#include "ComponentType.h"

class ComponentTypes {
	public:
		static const int NUM_TYPES = 3;

		static const ComponentType WIRE,
			RESISTOR,
			VSRC;

	public:
		ComponentTypes() {};
		ComponentTypes(ComponentTypes const&) = delete;
		void operator =(ComponentTypes const&) = delete;
		~ComponentTypes() = default;

		// Static helper methods
		static const ComponentType& getType(int index);
};

