/**
    Author:             Matthew Olsson
    File Title:         Node.h
    File Description:   Implements the Node and KCLTerm structs, along with
                        various enums. The Node struct represents a fully
                        abstract circuit node for use in nodal analysis. The
                        KCLTerm struct represents one term of KCL performed
                        at a node during nodal analysis.
    Due Date:           4/25/2018
    Date Created:       4/13/2018
    Date Last Modified: 4/23/2018
*/

#include "Node.h"

Node::Node(int id_) {
    id = id_;
}

void Node::addConnection(Node* node, double value, Unit type, Polarity polarity) {
    connections.push_back(std::make_tuple(node, value, type, polarity));
}

void Node::removeConnection(Node* node, double value, Unit type, Polarity polarity) {
    // Loop over the connections and match a single connection that matches the
    // arguments. It is important that only a single connection is removed, as
    // Nodes can have multiples connections that are the same (eg: Two 
    // five-ohm resistors connected in the same direction from one node to
    // another).
    bool found = false;
    for (int i = 0; i < connections.size() && !found; i++) {
        if (connections[i] == std::make_tuple(node, value, type, polarity)) {
            connections.erase(connections.begin() + i);
            found = true;
        }
    }
}

void Node::removeConnection(std::tuple<Node*, double, Unit, Polarity> t) {
    // Loop over the connections and match a single connection that matches the
    // arguments. It is important that only a single connection is removed, as
    // Nodes can have multiples connections that are the same (eg: Two 
    // five-ohm resistors connected in the same direction from one node to
    // another).
    bool found = false;
    for (int i = 0; i < connections.size() && !found; i++) {
        if (connections[i] == t) {
            connections.erase(connections.begin() + i);
            found = true;
        }
    }
}

void Node::removeConnections(Node* node) {
    // Loop over the connections and erase any connections between this node
    // and the argument.
    auto it = connections.begin();

    while (it != connections.end()) {
        if (std::get<0>(*it) == node) {
            it = connections.erase(it);
        } else {
            ++it;
        }
    }
}

std::vector<std::tuple<Node*, double, Unit, Polarity>> Node::getConnectionsToNode(Node* node) const {
    std::vector<std::tuple<Node*, double, Unit, Polarity>> connects;

    // Loop over all connections and return the nodes that match the argument.
    for (auto t : connections) {
        if (std::get<0>(t)->id == node->id)
            connects.push_back(t);
    }

    return connects;
}

std::vector<KCLTerm> Node::getTerms(const Node* callingNode, std::vector<Node*>& excludedNodes) const {
    std::vector<KCLTerm> terms;

    for (auto connection : connections) {

        // If the connection type is a voltage source, it must be treated as a
        // supernode. This means that the terms from the node on the other end
        // of the voltage source must be gathered and merged into the terms 
        // this method returns. If this is a top-level call (ie: called 
        // directly from Calculator::calculate with an argument of nullptr), we
        // automatically gather all the terms from the opposing node. If the 
        // call was made recursively (ie: sub-top-level), the callingNode will
        // be populated with the caller. Only get the terms of the connected 
        // node if it is not the caller (otherwise, it would form an infinite 
        // loop).
        if (std::get<2>(connection) == Unit::VOLT && (callingNode == nullptr || std::get<0>(connection)->id != callingNode->id)) {
            excludedNodes.push_back(std::get<0>(connection));
            std::vector<KCLTerm> newTerms = std::get<0>(connection)->getTerms(this, excludedNodes);
            terms.insert(terms.end(), newTerms.begin(), newTerms.end());
        } else if (std::get<2>(connection) == Unit::OHM) {
            // For resistors, the KCL term looks like (posNodeVoltage - 
            // negNodeVoltage) / resistorValue.
            const Node* pos;
            const Node* neg;

            if (std::get<3>(connection) == Polarity::POSITIVE) {
                pos = this;
                neg = std::get<0>(connection);
            } else {
                pos = std::get<0>(connection);
                neg = this;
            }

            terms.push_back({ pos->id, neg->id, std::get<1>(connection), pos->id == id});
        }
    }

    return terms;
}

bool Node::operator ==(Node other) const {
    return id == other.id;
}

Polarity operator !(Polarity pol) {
    Polarity notPol = Polarity::UNKNOWN;

    if (pol == Polarity::POSITIVE) {
        notPol = Polarity::NEGATIVE;
    } else if (pol == Polarity::NEGATIVE) {
        notPol = Polarity::POSITIVE;
    }

    return notPol;
}