/**
	Author:                 Matthew Olsson
	Assignment Title:       Group Project - Circuit Simulator
	Assignment Description: This application allows the simulation of simple circuits
							that consist of only wires, resistor, and voltage sources.
	Due Date:               4/25/2018
	Date Created:           3/16/2018
	Date Last Modified:     3/23/2018
*/

#include "ApplicationManager.h"
#include "Config.h"

int main() {
	ApplicationManager am(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Circuit Simluator");

	while (am.isOpen()) {
		am.update();
	}

	return 0;
}