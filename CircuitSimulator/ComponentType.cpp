#include "ComponentType.h"

ComponentType::ComponentType(int v, std::string n) {
    value = v;
    name = n;
}

int ComponentType::getValue() const {
    return value;
}

std::string ComponentType::getName() const {
    return name;
}