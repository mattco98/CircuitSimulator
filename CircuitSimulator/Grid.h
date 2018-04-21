#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "GridSpot.h"

class Grid { 
	private:
        int width,
            height;

		std::vector<Component*> components;
		std::vector< std::vector<GridSpot*> > spots;

	public:
		Grid();

		inline std::vector<Component*> getComponents() const { return components; };
		inline std::vector< std::vector<GridSpot*> > getSpots() const { return spots; };
		void clearComponents() { components.clear(); };
		void addComponent(Component* component);
		bool getNearestSpot(sf::Vector2i mousePos, GridSpot** spot) const;
        bool getComponentUnderPosition(sf::Vector2i pos, Component*&) const;
};
