#pragma once

#include <string>
#include "ComponentTypes.h"

struct GridSpot;

class Component {
	private:
		const ComponentType* type;

        double value;
		double voltageDrop;
		double ampsThrough;

		GridSpot* positive;
		GridSpot* negative;

	public:
		Component(const ComponentType* type);

        const ComponentType* getType();
        double getValue();
        double getVoltageDrop();
		double getAmpsThrough();
		GridSpot* getPositive();
		GridSpot* getNegative();
        GridSpot* getOther(GridSpot* s);

        void setValue(double v);
		void setVoltageDrop(double drop);
		void setAmpsThrough(double amps);

		void setPositive(GridSpot** spot);
		void setNegative(GridSpot** spot);
};