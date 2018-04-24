/**
    Author:             Matthew Olsson
    File Title:         ComponentTypes.h
    File Description:   Declares and implements the ComponentType struct, along
                        with the valid component types. This header file serves
                        as a sort of Component Type Enum.
    Due Date:           4/25/2018
    Date Created:       3/25/2018
    Date Last Modified: 4/23/2018
*/

#pragma once

#include <string>
#include <SFML/Window/Keyboard.hpp>

struct ComponentType {
    std::string name;

    /**
        Description:   Initializes a ComponentType object with the provided
                       name.
        Return:        None
        Precondition:  None
        Postcondition: A ComponentType object will be returned with the 
                       provided name.
    */
    ComponentType(std::string);

    /**
        Description:   Returns the name of this ComponentType;
        Return:        string
        Precondition:  This object exists.
        Postcondition: The name will be returned. This object will not be
                       modified.
    */
    std::string getName() const;
};

extern const ComponentType WIRE;
extern const ComponentType RESISTOR;
extern const ComponentType VSRC;

/**
    Description:   Shifts the provided component type pointer given a
                   direction.
    Return:        void
    Precondition:  The ComponentType pointer reference is valid, and the key
                   has been properly set.
    Postcondition: The ComponentType pointer will have been changed. The key
                   will not be modified.
*/
void shift(const ComponentType*&, sf::Keyboard::Key);