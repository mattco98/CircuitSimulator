/**
	Author:                 Matthew Olsson
	Assignment Title:       Group Project - Circuit Simulator
	Assignment Description: This application allows the simulation of simple 
                            circuits that consist of only wires, resistors, and
                            voltage sources.
	Due Date:               4/25/2018
	Date Created:           3/16/2018
	Date Last Modified:     4/23/2018
*/

#include "ApplicationManager.h"
#include "Config.h"

int main() {
    // Open an instance of the application
	ApplicationManager am(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Circuit Simluator");

    // While the application is open, update the application manager
	while (am.isOpen()) {
		am.update();
	}

	return 0;
}