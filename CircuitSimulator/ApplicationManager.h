#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Component.h"
#include "Config.h"

class ApplicationManager {
	private:
		std::vector<sf::Texture> textures;
		bool isPlacing = false;
		ComponentType selectedComponentType;
		Component* selectedComponent;

		Grid grid;
		sf::RenderWindow window;

	public:
		ApplicationManager(sf::VideoMode mode, std::string windowTitle, sf::Uint32 style = sf::Style::Default);
		~ApplicationManager();

		inline bool isOpen() { return window.isOpen(); };
		void update();


	private:

		////////////////////////
		// Management Methods //
		////////////////////////
		void handleEvent(sf::Event event);
		void handleKeypress(sf::Event::KeyEvent event);
		void handleMousePressed(sf::Event::MouseButtonEvent event);
		void handleResized(sf::Event event);
		void handleClosed();
		ComponentType getSelectedComponentType();
		void setSelectedComponentType(ComponentType);


		///////////////////////
		// Rendering methods //
		///////////////////////
		void draw(); // Main draw method, all drawing is done here. Called via update
		void drawGui();
		void drawComponentMenu();
		void drawComponent(Component* component);
		void drawWire(Component* component);
		void drawResistor(Component* component);
		void drawVSrc(Component* component);


		////////////////////
		// Helper Methods //
		////////////////////
		void drawLine(sf::Vertex p1, sf::Vertex p2);
		void drawLine(sf::Vector2f p1, sf::Vector2f p2, sf::Color = sf::Color::White);
		void drawRectangleHollow(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4, sf::Color color);
};

