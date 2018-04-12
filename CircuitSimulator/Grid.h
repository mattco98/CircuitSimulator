#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "Spot.h"

class Grid { 
	private:
		int width,
			height,
			xOffset,
			yOffset;

		std::vector<Component*> components;
		std::vector<Spot*> spots;

	public:
		Grid(int width, int height, int xOffset = 0, int yOffset = 0);
		~Grid();

		inline auto getComponents() { return components; };
		inline auto getSpots() { return spots; };
		void clearComponents() { components.clear(); };
		void addComponent(Component* component);
		bool getNearestSpot(sf::Vector2i mousePos, Spot** spot);
};
