/**
    Author:             Matthew Olsson
    File Title:         ApplicationManager.h
    File Description:   Declares the ApplicationManager class, the class
                        responsible for handling the application. Primary
                        tasks are managing the window inputs and outputs, and
                        coordinating the grid of components.
    Due Date:           4/25/2018
    Date Created:       3/16/2018
    Date Last Modified: 4/23/2018
*/

#pragma once

#include <string>                // string class
#include <vector>                // vector class, .push_back(), .at(), 
#include <SFML/Graphics.hpp>     // SFML Graphics header. Objects: Texture
                                 // RenderWindow, Event (and sub-events),
                                 // Keyboard, Vector2i/f, Color, CircleShape,
                                 // RectangleShape, Text, Sprite, Vertex, and
                                 // associated methods.
#include "Grid.h"
#include "Component.h"
#include "Config.h"

/**
    This enum tracks the current state of the application. There
    are five distinct modes:
        PLACING:
            The user is able to place the first point of a new component.
        PLACING_COMPONENT:
            The user has already placed one end of a new component, and is able
            to place the second end.
        SELECTING:
            The user is able to select a component a view it's information.
        SELECTED:
            The user has selected a component.
        TYPING:
            The user is entering a component's value.
*/
enum Mode {
    PLACING,
    PLACING_COMPONENT,
    SELECTING,
    SELECTED,
    TYPING
};

class ApplicationManager {
	private:
        // Tracks whether or not the current drawn circuit is valid. If not,
        // an error is displayed.
        bool error = false;

        // Stores the application mode.
        Mode mode = PLACING;

        // A string for storing application input. Used for inputting the value
        // of a component
        std::string input = "";

        // A vector that holds component textures used for the rendering of the
        // component on the GUI. The textures are initialized in the 
        // constructor.
		std::vector<sf::Texture> textures;

        // The type that the user currently has selected.
		const ComponentType* placingComponentType;

        // Used to draw a dummy component while the user is mid-place.
		Component* placingComponent;

        // The component that the user has selected.
        Component* selectedComponent;

        // Grid object instance.
		Grid grid;

        // Instance of an SFML window object, which is what is displayed to the
        // user.
		sf::RenderWindow window;

	public:
        /**
            Description:   Constructs a new application.
            Returns:       None
            Precondition:  None
            Postcondition: An instance of this class is returned with the 
                           provided arguments used to intialize the underlying
                           SFML RenderWindow. All fields will have been 
                           properly initialized. The arguments will not be
                           modified.
        */
		ApplicationManager(sf::VideoMode, std::string, sf::Uint32 = sf::Style::Default);

        /**
            Description:   Returns whether or not the Window object is still
                           open. Serves as the main loop condition.
            Returns:       bool
            Precondition:  The Window object is correctly initialized, and this
                           object exists.
            Postcondition: The open status of the Window will be returned. This
                           object will not be modified.
        */
		inline bool isOpen() { return window.isOpen(); };

        /**
            Description:   Redraws the window and polls for available events 
                           (eg: Mouse clicks, keyboard input, etc).
            Returns:       void
            Precondition:  This object exists.
            Postcondition: The application and the user's display will have
                           been updated depending on previous state and current
                           input.
        */
		void update();


	private:

		////////////////////////
		// Management Methods //
		////////////////////////

        /**
            Description:   Handles all incoming SFML events (eg: Mouse clicks,
                           keyboard input, etc). Triggered every time any SFML
                           event is detected.
            Returns:       void
            Precondition:  This object exists. The provided event has been
                           correctly polled from SFML's input buffer. 
            Postcondition: The application and the user's display will have
                           been updated depending on previous state and current
                           input. The event will have been properly handled.
                           The argument will not be modified.
        */
		void handleEvent(sf::Event);

        /**
            Description:   Handles all incoming TextEvents. Triggered on any
                           keyboard key press.
            Returns:       void
            Precondition:  This object exists. The provided event has been
                           correctly polled from SFML's input buffer.
            Postcondition: The application and the user's display will have
                           been updated depending on previous state and current
                           input. The event will have been properly handled.
                           The argument will not be modified.
        */
        void handleTextEntered(sf::Event::TextEvent);

        /**
            Description:   Handles all incoming TextEvents. Triggered on any
                           keyboard key press.
            Returns:       void
            Precondition:  This object exists. The provided event has been
                           correctly polled from SFML's input buffer.
            Postcondition: The application and the user's display will have
                           been updated depending on previous state and current
                           input. The event will have been properly handled.
                           The argument will not be modified.
        */
		void handleKeypress(sf::Event::KeyEvent);

        /**
            Description:   Handles all incoming TextEvents. Triggered on any
                           mouse button press.
            Returns:       void
            Precondition:  This object exists. The provided event has been
                           correctly polled from SFML's input buffer.
            Postcondition: The application and the user's display will have
                           been updated depending on previous state and current
                           input. The event will have been properly handled.
                           The argument will not be modified.
        */
		void handleMousePressed(sf::Event::MouseButtonEvent);


		///////////////////////
		// Rendering methods //
		///////////////////////

        /**
            Description:   The main drawing method. This method is called
                           every frame from update(), and calls every other
                           draw method when appropriate. Responsible for 
                           everything drawn to the user's display.
            Returns:       void
            Precondition:  This object exists.
            Postcondition: The user's display will have been updated depending
                           on previous state and current input. The state of
                           this object, aside from the current state of the
                           window, will not have been modified.
        */
		void draw();

        /**
            Description:   Drawing the application GUI. This consists of the
                           left sidebar of information, as well as the border
                           around the main grid.
            Returns:       void
            Precondition:  This object exists.
            Postcondition: The user's display will have been updated depending
                           on previous state and current input. The state of
                           this object, aside from the current state of the
                           window, will not have been modified.
        */
		void drawGui();

        /**
            Description:   Draws the title panel, the topmost panel in the left
                           sidebar. Contains the title of the application.
            Returns:       void
            Precondition:  This object exists.
            Postcondition: The user's display will have been updated depending
                           on previous state and current input. The state of
                           this object, aside from the current state of the
                           window, will not have been modified.
        */
        void drawTitlePanel();

        /**
            Description:   Draws the component panel below the title bar.
                           Contains the possible component types the user can
                           select when placing components.
            Returns:       void
            Precondition:  This object exists.
            Postcondition: The user's display will have been updated depending
                           on previous state and current input. The state of
                           this object, aside from the current state of the
                           window, will not have been modified.
        */
		void drawComponentPanel();

        /**
            Description:   Draws the instruction panel below the component
                           panel. Displays the current mode the user is in and
                           provides a brief description of what they can do.
            Returns:       void
            Precondition:  This object exists.
            Postcondition: The user's display will have been updated depending
                           on previous state and current input. The state of
                           this object, aside from the current state of the
                           window, will not have been modified.
        */
        void drawInstructionPanel();

        /**
            Description:   Draws the information panel below the instruction
                           panel. This displays informatino about the currently
                           selected component, including voltage and current,
                           and the component value, if it is not a wire.
            Returns:       void
            Precondition:  This object exists.
            Postcondition: The user's display will have been updated depending
                           on previous state and current input. The state of
                           this object, aside from the current state of the
                           window, will not have been modified.
        */
        void drawInfoPanel();

        /**
            Description:   Draws the specified component at its two points on
                           on the grid. The second argument determines the
                           component color, defaults to COMPONENT_COLOR.
            Returns:       void
            Precondition:  This object exists, and the component passed in has
                           been correctly initialized and its spots have been
                           set.
            Postcondition: The user's display will have been updated depending
                           on previous state and current input. The state of
                           this object, aside from the current state of the
                           window, will not have been modified. The arguments
                           will not be modified.
        */
		void drawComponent(Component*, sf::Color = COMPONENT_COLOR);

        /**
            Description:   Draws a hollow rectangle at the specified positions
                           in the specified color. Draws the lines in the order
                           they were provided: v1 to v2, v2 to v3, v3 to v4, 
                           and v4 to v1.
            Returns:       void
            Precondition:  This object exists, and all vectors have coordinates
                           that are within the bounds of the window.
            Postcondition: The user's display will have been updated depending
                           on previous state and current input. The state of
                           this object, aside from the current state of the
                           window, will not have been modified. The arguments
                           will not be modified.
        */
        void drawRectangleHollow(sf::Vector2f v1, sf::Vector2f v2, sf::Vector2f v3, sf::Vector2f v4, sf::Color);

        /**
            Description:   Draws a line from the first vector to the second 
                           with the specified color.
            Returns:       void
            Precondition:  This object exists, and all vectors have coordinates
                           that are within the bounds of the window.
            Postcondition: The user's display will have been updated depending
                           on previous state and current input. The state of
                           this object, aside from the current state of the
                           window, will not have been modified. The arguments
                           will not be modified.
        */
        void drawLine(sf::Vector2f, sf::Vector2f, sf::Color);


		////////////////////
		// Helper Methods //
		////////////////////

        /**
            Description:   Handles setting the selectedComponent value from a
                           string input by the user. Utilized by the 
                           handleTextEntered() method.
            Returns:       void
            Precondition:  This object exists, and the passed in string is
                           valid.
            Postcondition: The application and the user's display will have
                           been updated depending on previous state and current
                           input. The argument will not be modified.
        */
        void setSelectedComponentValue(std::string);

        /**
            Description:   Recalculates all component values. Catches any
                           errors that are thrown when calculating the circuit,
                           and enables the application's error state if this
                           occurs.
            Returns:       void
            Precondition:  This object exists.
            Postcondition: The application and the user's display will have
                           been updated depending on previous state and current
                           input.
        */
        void recalculate();
};