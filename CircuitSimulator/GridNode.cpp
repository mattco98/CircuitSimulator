/**
    Author:             Matthew Olsson
    File Title:         GridNode.h
    File Description:   Implements the GridNode struct. This struct represents
                        a "rough node" (see Node.h). In other words, it is the
                        first step in the process of performing nodal analysis
                        on the circuit built by the user.
    Due Date:           4/25/2018
    Date Created:       4/10/2018
    Date Last Modified: 4/23/2018
*/

#include "GridNode.h"

std::vector<Component*> GridNode::getNonWireComponents() const {
    std::vector<Component*> components;

    // Iterate through all spots in this node
    for (GridSpot* spot : spots) {
        std::vector<Component*> comps = spot->components;

        // Iterate through all components in this spot
        for (Component* comp : comps) {

            // Ensure the component isn't a wire
            if (comp->type != &WIRE) {

                // Make sure we haven't seen this component before to avoid
                // duplicate entries.
                bool seen = false;
                for (Component* comp1 : components)
                    if (comp1 == comp) seen = true;
                if (!seen)
                    components.push_back(comp);
            }
        }
    }

    return components;
}