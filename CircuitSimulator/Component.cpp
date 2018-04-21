#include "Component.h"

Component::Component(const ComponentType* t) {
    type = t;
    value = 5.0;
    voltageDrop = 5.0;
    ampsThrough = 0.0;
};

const ComponentType* Component::getType() {
    return type;
};

double Component::getValue() {
    return value;
}

double Component::getVoltageDrop() {
    return voltageDrop;
};

double Component::getAmpsThrough() {
    return ampsThrough;
};

GridSpot* Component::getPositive() {
    return positive;
};

GridSpot* Component::getNegative() {
    return negative;
};

GridSpot* Component::getOther(GridSpot* s) {
    return s == positive ? negative : (s == negative ? positive : nullptr);
}

void Component::setValue(double v) {
    value = v;
}

void Component::setVoltageDrop(double drop) {
    voltageDrop = drop;
};

void Component::setAmpsThrough(double amps) {
    ampsThrough = amps;
};

void Component::setPositive(GridSpot** spot) {
    positive = *spot;
};

void Component::setNegative(GridSpot** spot) {
    negative = *spot;
};