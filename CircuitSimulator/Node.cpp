#include "Node.h"

Node::Node(int id_) {
    id = id_;
}

void Node::addConnection(Node* node, double value, Unit type, Polarity polarity) {
    connections.push_back(std::make_tuple(node, value, type, polarity));
}

void Node::removeConnection(Node* node, double value, Unit type, Polarity polarity) {
    for (int i = 0; i < connections.size(); i++) {
        if (connections[i] == std::make_tuple(node, value, type, polarity)) {
            connections.erase(connections.begin() + i);
            return;
        }
    }
}

void Node::removeConnection(std::tuple<Node*, double, Unit, Polarity> t) {
    for (int i = 0; i < connections.size(); i++) {
        if (connections[i] == t) {
            connections.erase(connections.begin() + i);
            return;
        }
    }
}

void Node::removeConnections(Node* node) {
    for (int i = 0; i < connections.size(); i++) {
        if (std::get<0>(connections[i]) == node) {
            connections.erase(connections.begin() + i);
        }
    }
}

std::vector< std::tuple<Node*, double, Unit, Polarity> > Node::getConnectionsToNode(Node* node) const {
    std::vector< std::tuple<Node*, double, Unit, Polarity> > connects;
    for (auto t : connections) {
        if (std::get<0>(t)->id == node->id)
            connects.push_back(t);
    }
    return connects;
}

void Node::print(std::ostream& stream) const {
    stream << "Node " << id << "\n";
    for (int i = 0; i < connections.size(); i++) {
        stream << "\tConnection " << i << ":\n";
        auto connection = connections[i];
        stream << "\t\tOther Node: " << std::get<0>(connection)->id << std::endl;
        stream << "\t\tConnection Type: " << (std::get<2>(connection) == Unit::OHM ? "Ohm" : "Volt") << std::endl;
        stream << "\t\tConnection Value: " << std::get<1>(connection) << std::endl;
        stream << "\t\tConnection Polarity: " << (std::get<3>(connection) == Polarity::POSITIVE ? "Positive" : std::get<3>(connection) == Polarity::NEGATIVE ? "Negative" : "Unknown") << "\n\n";
    }
}

std::vector<KCLTerm> Node::getTerms(const Node* callingNode, std::vector<Node*>& excludedNodes) const {
    std::vector<KCLTerm> terms;
    for (auto connection : connections) {
        // If the connection type is a voltage source, we need to treat it as a supernode.
        // This means that the terms from the node on the other end of the voltage source
        // must be gathered and merged into the terms this method returns. If this is a 
        // top-level call (ie: called directly from Calculator::calculate with an argument
        // of nullptr), we automatically gather all the terms from the opposing node. If
        // the call was made recursively (ie: sub-top-level), the callingNode will be 
        // populated with the caller. Only get the terms of the connected node if it is
        // not the caller (otherwise, it would form an infinite loop).
        if (std::get<2>(connection) == Unit::VOLT && (callingNode == nullptr || std::get<0>(connection) != callingNode)) {
            excludedNodes.push_back(std::get<0>(connection));
            std::vector<KCLTerm> newTerms = std::get<0>(connection)->getTerms(this, excludedNodes);
            terms.insert(terms.end(), newTerms.begin(), newTerms.end());
        } else if (std::get<2>(connection) == Unit::OHM) {

            const Node* pos;
            const Node* neg;

            if (std::get<3>(connection) == Polarity::POSITIVE) {
                pos = this;
                neg = std::get<0>(connection);
            } else {
                pos = std::get<0>(connection);
                neg = this;
            }

            terms.push_back({ pos->id, neg->id, std::get<1>(connection) });
        }
    }

    return terms;
}

bool Node::operator ==(Node other) const {
    return id == other.id;
}

Polarity operator !(Polarity pol) {
    if (pol == Polarity::POSITIVE) {
        return Polarity::NEGATIVE;
    } else if (pol == Polarity::NEGATIVE) {
        return Polarity::POSITIVE;
    } else {
        return Polarity::UNKNOWN;
    }
}