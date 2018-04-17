#pragma once

#include <string>
#include "ComponentTypes.h"

struct GridSpot;

class Component {
	private:
		const ComponentType* type;
		double voltageDrop;
		double ampsThrough;

		GridSpot* positive;
		GridSpot* negative;

	public:
		Component() = delete;
		Component(const ComponentType* type) : type(type), voltageDrop(0.0), ampsThrough(0.0) {};

		inline const ComponentType* getType() { return type; };
		inline double getVoltageDrop() { return voltageDrop; };
		inline double getAmpsThrough() { return ampsThrough; };
		inline GridSpot* getPositive() { return positive; };
		inline GridSpot* getNegative() { return negative; };

		inline void setVoltageDrop(double drop) { voltageDrop = drop; };
		inline void setAmpsThrough(double amps) { ampsThrough = amps; };

		inline void setPositive(GridSpot** spot) { positive = *spot; };
		inline void setNegative(GridSpot** spot) { negative = *spot; };
};