#pragma once

#include <string>

struct ComponentType {
    int value;
    std::string name;

    ComponentType(int, std::string);

    int getValue() const;
    std::string getName() const;
};