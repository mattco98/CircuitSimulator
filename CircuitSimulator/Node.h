#pragma once

#include <iostream>
#include <vector>
#include "GridSpot.h"

struct Node {
    std::vector<GridSpot*> spots;

    void print(std::ostream&) const;
};