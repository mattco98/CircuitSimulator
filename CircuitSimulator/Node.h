#pragma once

#include <iostream>
#include <vector>
#include <tuple>

enum class ValueType {
    OHM = 0,
    VOLT = 1
};

enum class Polarity {
    UNKNOWN = 0,
    POSITIVE = 1,
    NEGATIVE = 2
};

Polarity operator !(Polarity pol);

struct Node {
    int id;
    std::vector< std::tuple<Node*, double, ValueType, Polarity> > connections;

    Node(int);

    void addConnection(Node*, double, ValueType, Polarity);
    void removeConnection(Node*, double, ValueType, Polarity);
    void removeConnection(std::tuple<Node*, double, ValueType, Polarity>);
    void removeConnections(Node*);
    std::vector< std::tuple<Node*, double, ValueType, Polarity> > getConnectionsToNode(Node* node) const;

    void print(std::ostream&) const;
};