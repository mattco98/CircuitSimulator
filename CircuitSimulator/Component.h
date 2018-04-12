#pragma once

#include <string>
#include "ComponentType.h"

struct Spot;

class Component {
	private:
		const ComponentType type;
		double voltageDrop;
		double ampsThrough;

		Spot* positive;
		Spot* negative;

	public:
		Component() = delete;
		Component(ComponentType type) : type(type) {};

		Component(const Component& cmpt);
		~Component() = default;

		inline const ComponentType getType() { return type; };
		inline double getVoltageDrop() { return voltageDrop; };
		inline double getAmpsThrough() { return ampsThrough; };
		inline Spot* getPositive() { return positive; };
		inline Spot* getNegative() { return negative; };

		inline void setVoltageDrop(double drop) { voltageDrop = drop; };
		inline void setAmpsThrough(double amps) { ampsThrough = amps; };

		inline void setPositive(Spot** spot) { positive = *spot; };
		inline void setNegative(Spot** spot) { negative = *spot; };
};