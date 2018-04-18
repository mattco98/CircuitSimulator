#pragma once

#include <vector>
#include "Component.h"
#include "GridSpot.h"

typedef std::vector< std::vector<GridSpot*> > spot_vec;

namespace Calculator {
    bool isCompleteCircuit(spot_vec&, int);
}