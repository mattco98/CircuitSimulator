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
		ApplicationManager(sf::VideoMode mode, std::string windowTitle, sf::Uint32 style = sf::Style::Default);

		inline bool isOpen() { return window.isOpen(); };
		void update();


	private:

		////////////////////////
		// Management Methods //
		////////////////////////
		void handleEvent(sf::Event event);
        void handleTextEntered(sf::Event::TextEvent event);
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
        void setSelectedComponentValue(std::string);
};