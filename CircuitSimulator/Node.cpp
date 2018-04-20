#include "Node.h"

void Node::print(std::ostream& stream) const {
    std::cout << "Node members:\n";

    for (int i = 0; i < spots.size(); i++) {
        GridSpot* spot = spots[i];
        std::cout << "\t(" << spot->x << ", " << spot->y << ") with " << spot->components.size() << " components\n";
    }

    std::cout << "\n";
}