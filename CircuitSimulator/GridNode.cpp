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