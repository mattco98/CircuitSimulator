#pragma once

#include <iostream>
#include <vector>
#include "GridSpot.h"

struct GridSpot;

struct Node {
    std::vector<GridSpot*> spots;

    inline void print(std::ostream& stream) const {
        std::cout << "Node members:\n";

        for (int i = 0; i < spots.size(); i++) {
            GridSpot* spot = spots[i];
            std::cout << "\t(" << spot->x << ", " << spot->y << ") with " << spot->components.size() << " components";
        }

        std::cout << "\n";
    }
};