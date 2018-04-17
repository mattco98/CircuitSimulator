#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Component.h"
#include "Component.h"
#include "Config.h"

class ApplicationManager {
	private:
        //int alphaTransition = 0;
        double alpha = 255.0;

		std::vector<sf::Texture> textures;

		bool isPlacing = false;
		const ComponentType* placingComponentType;
		Component* placingComponent;
        Component* selectedComponent;

		Grid grid;
		sf::RenderWindow window;

	public:
		ApplicationManager(sf::VideoMode mode, std::string windowTitle, sf::Uint32 style = sf::Style::Default);

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


		///////////////////////
		// Rendering methods //
		///////////////////////
		void draw(); // Main draw method, all drawing is done here. Called via update
		void drawGui();
        void drawTitlePanel();
		void drawComponentPanel();
        void drawInstructionPanel();
        void drawInfoPanel();
		void drawComponent(Component* component);
		void drawWire(Component* component);
		void drawResistor(Component* component);
		void drawVSrc(Component* component);
        //void transitionAlpha();


		////////////////////
		// Helper Methods //
		////////////////////
};