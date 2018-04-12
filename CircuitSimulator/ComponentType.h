#pragma once

#include <string>

struct ComponentType {
	int value;
	std::string name;

	ComponentType(int value, std::string name) : value(value), name(name) {};

	int getValue() const {
		return value;
	}

	std::string getName() const {
		return name;
	}
};