#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Component.h"
#include "Component.h"
#include "Config.h"

enum Mode {
    PLACING = 0,
    PLACING_COMPONENT = 1,
    SELECTING = 2,
    SELECTED = 3,
    TYPING = 4
};

class ApplicationManager {
	private:
        bool error = false;
        Mode mode = PLACING;
        std::string input = "";

        //int alphaTransition = 0;
        double alpha = 255.0;

		std::vector<sf::Texture> textures;

		const ComponentType* placingComponentType;
		Component* placingComponent;
        Component* selectedComponent;

		Grid grid;
		sf::RenderWindow window;

	public:
		ApplicationManager(sf::VideoMode, std::string, sf::Uint32 = sf::Style::Default);

		inline bool isOpen() { return window.isOpen(); };
		void update();


	private:

		////////////////////////
		// Management Methods //
		////////////////////////
		void handleEvent(sf::Event);
        void handleTextEntered(sf::Event::TextEvent);
		void handleKeypress(sf::Event::KeyEvent);
		void handleMousePressed(sf::Event::MouseButtonEvent);
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
		void drawComponent(Component*, sf::Color = COMPONENT_COLOR);
		void drawWire(Component*, sf::Color = COMPONENT_COLOR);
		void drawResistor(Component*, sf::Color = COMPONENT_COLOR);
		void drawVSrc(Component*, sf::Color = COMPONENT_COLOR);
        //void transitionAlpha();


		////////////////////
		// Helper Methods //
		////////////////////
        void setSelectedComponentValue(std::string);
        void recalculate();
};