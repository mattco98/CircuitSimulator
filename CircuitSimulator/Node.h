/**
    Author:             Matthew Olsson
    File Title:         Node.h
    File Description:   Declares the Node and KCLTerm structs, along with
                        various enums. The Node struct represents a fully 
                        abstract circuit node for use in nodal analysis. The
                        KCLTerm struct represents one term of KCL performed
                        at a node during nodal analysis.
    Due Date:           4/25/2018
    Date Created:       4/13/2018
    Date Last Modified: 4/23/2018
*/

#pragma once

#include <vector>   // Vector class, .push_back(), .erase(), .begin(), .end()
#include <tuple>    // Tuple class, make_tuple(), get<>()

/**
    The ComponentType of the nodal analysis process - keeps track of whether
    or not an element is a resistor or voltage source.
*/
enum class Unit {
    OHM,
    VOLT
};

/**
    Represents the polarity of the connection, used to keep track of signs. A
    connection with positive Polarity means that the positive end of the
    component is connected to this node.
*/
enum class Polarity {
    UNKNOWN,
    POSITIVE,
    NEGATIVE
};

/**
    Description:   Inverts the polarity.
    Return:        Polarity
    Precondition:  pol is a valid Polarity.
    Postcondition: If pol is POSITIVE, returns NEGATIVE. If pol is NEGATIVE,
                   returns POSITIVE. Does nothing is pol is UNKNOWN.
*/
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
    std::vector<std::tuple<Node*, double, Unit, Polarity>> connections;

    /**
        Description:   Initializes a Node object with no initial values.
        Return:        None
        Precondition:  None
        Postcondition: A Node object is returned with no default values.
    */
    Node() = default;

    /**
        Description:   Initializes a Node object with the given ID.
        Return:        None
        Precondition:  None
        Postcondition: A Node object is returned with the given ID. The int
                       will not be modified.
    */
    Node(int);

    /**
        Description:   Adds a connection to the connections vector.
        Return:        void
        Precondition:  This object exists, and all provided arguments are valid
                       and have been initialized.
        Postcondition: A tuple will be added to the connections vector. No
                       arguments will be modified.
    */
    void addConnection(Node*, double, Unit, Polarity);

    /**
        Description:   Removes a connection from the connections vector.
        Return:        void
        Precondition:  This object exists, and all provided arguments are valid
                       and have been initialized.
        Postcondition: A tuple will be removed from the connections vector if
                       one exists which matches all of the provided arguments.
                       No arguments will be modified.
    */
    void removeConnection(Node*, double, Unit, Polarity);

    /**
        Description:   Removes a connection from the connections vector
        Return:        void
        Precondition:  This object exists, and the provided tuple is valid and
                       has been initialized and populated.
        Postcondition: A tuple will be removed from the connections vector if
                       one exists which matches the provided tuple. No 
                       arguments will be modified.
    */
    void removeConnection(std::tuple<Node*, double, Unit, Polarity>);

    /**
        Description:   Removed all connections from the connections vector
                       whose other Node is the provided Node.
        Return:        void
        Precondition:  This object exists, and the provided Node points to a 
                       valid Node object.
        Postcondition: Any connections to the provided Node will be removed 
                       from the connections vector. The argument will not be
                       modified.
    */
    void removeConnections(Node*);

    /**
        Description:   Gets all connections this Node has with another Node.
        Return:        vector<tuple<Node*, double, Unit, Polarity>>
        Precondition:  This object exists and the provided Node points to a 
                       valid Node object.
        Postcondition: All connections this Node has with the Node passed in
                       will be returned.
    */
    std::vector<std::tuple<Node*, double, Unit, Polarity>> getConnectionsToNode(Node* node) const;

    /**
        Description:   Gets all Node KCLTerms.
        Return:        vector<KCLTerm>
        Precondition:  This object exists and the arguments passed in are 
                       valid.
        Postcondition: All KCLTerms from this Node will be returned.
    */
    std::vector<KCLTerm> getTerms(const Node*, std::vector<Node*>&) const;

    /**
        Description:   Determines equality with another node based on their
                       IDs.
        Return:        bool
        Precondition:  Both Nodes have been initialized with an ID.
        Postcondition: Returns true if the Node have the same ID, false
                       otherwise.
    */
    bool operator ==(Node other) const;
};