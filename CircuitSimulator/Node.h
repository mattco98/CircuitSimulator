#pragma once

#include <iostream>
#include <vector>
#include <tuple>

enum class Unit {
    OHM = 0,
    VOLT = 1
};

enum class Polarity {
    UNKNOWN = 0,
    POSITIVE = 1,
    NEGATIVE = 2
};

Polarity operator !(Polarity pol);

struct KCLTerm {
    int pos,
        neg;
    double res;
    bool out;
};

struct Node {
    int id;
    double voltage;
    std::vector< std::tuple<Node*, double, Unit, Polarity> > connections;

    Node(int);

    void addConnection(Node*, double, Unit, Polarity);
    void removeConnection(Node*, double, Unit, Polarity);
    void removeConnection(std::tuple<Node*, double, Unit, Polarity>);
    void removeConnections(Node*);
    std::vector< std::tuple<Node*, double, Unit, Polarity> > getConnectionsToNode(Node* node) const;
    std::vector<KCLTerm> getTerms(const Node*, std::vector<Node*>&) const;

    bool operator ==(Node other) const;

    void print(std::ostream&) const;
};