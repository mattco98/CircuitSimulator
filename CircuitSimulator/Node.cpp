#include "Node.h"

Node::Node(int id_) {
    id = id_;
}

void Node::addConnection(Node* node, double value, ValueType type, Polarity polarity) {
    connections.push_back(std::make_tuple(node, value, type, polarity));
}

void Node::removeConnection(Node* node, double value, ValueType type, Polarity polarity) {
    for (int i = 0; i < connections.size(); i++) {
        if (connections[i] == std::make_tuple(node, value, type, polarity)) {
            connections.erase(connections.begin() + i);
            return;
        }
    }
}

void Node::removeConnection(std::tuple<Node*, double, ValueType, Polarity> t) {
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

std::vector< std::tuple<Node*, double, ValueType, Polarity> > Node::getConnectionsToNode(Node* node) const {
    std::vector< std::tuple<Node*, double, ValueType, Polarity> > connects;
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
        stream << "\t\tConnection Type: " << (std::get<2>(connection) == ValueType::OHM ? "Ohm" : "Volt") << std::endl;
        stream << "\t\tConnection Value: " << std::get<1>(connection) << std::endl;
        stream << "\t\tConnection Polarity: " << (std::get<3>(connection) == Polarity::POSITIVE ? "Positive" : std::get<3>(connection) == Polarity::NEGATIVE ? "Negative" : "Unknown") << "\n\n";
    }
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