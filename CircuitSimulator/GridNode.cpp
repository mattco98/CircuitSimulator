#include "GridNode.h"

std::vector<Component*> GridNode::getNonWireComponents() const {
    std::vector<Component*> components;

    for (GridSpot* spot : spots) {
        std::vector<Component*> comps = spot->components;
        for (Component* comp : comps) {
            if (comp->getType() != &ComponentTypes::WIRE) {
                bool seen = false;
                for (Component* comp1 : components) {
                    if (comp1 == comp) seen = true;
                }
                if (!seen) {
                    components.push_back(comp);
                }
            }
        }
    }

    return components;
}

void GridNode::print(std::ostream& stream) const {
    std::cout << "Node members:\n";

    for (int i = 0; i < spots.size(); i++) {
        GridSpot* spot = spots[i];
        std::cout << "\t(" << spot->x << ", " << spot->y << ") with " << spot->components.size() << " components\n";
    }

    std::cout << "\n";
}