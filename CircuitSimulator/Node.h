#pragma once

#include <iostream>
#include <vector>
#include <tuple>

enum class ValueType {
    OHM = 0,
    VOLT = 1
};

struct Node {
    int id;
    std::vector< std::tuple<Node*, double, ValueType> > connections;

    Node(int);

    void addConnection(Node*, double, ValueType);
    void removeConnection(Node*, double, ValueType);
    void removeConnection(std::tuple<Node*, double, ValueType>);
    void removeConnections(Node*);
    std::vector< std::tuple<Node*, double, ValueType> > getConnectionsToNode(Node* node) const;

    void print(std::ostream&) const;
};