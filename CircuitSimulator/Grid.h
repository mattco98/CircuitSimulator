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

		std::vector<Component*> getComponents() const;
		std::vector< std::vector<GridSpot*> > getSpots() const;
        void clearComponents();
		void addComponent(Component* component);
		bool getNearestSpot(sf::Vector2i mousePos, GridSpot** spot) const;
        bool getComponentUnderPosition(sf::Vector2i pos, Component*&) const;
};
