/**
    Author:             Matthew Olsson
    File Title:         ApplicationManager.h
    File Description:   Implements the ApplicationManager class, the class
                        responsible for handling the application. Primary
                        tasks are managing the window inputs and outputs, and
                        coordinating the grid of components.
    Due Date:           4/25/2018
    Date Created:       3/16/2018
    Date Last Modified: 4/23/2018
*/

#include <iostream> // TODO remove
#include <iomanip>               // setprecision, fixed, left
#include <cmath>                 // pow, sqrt
#include <regex>                 // reg, smatch, regex_search, .size(), .str()
#include <sstream>               // stringstream class, .str()
#include "ApplicationManager.h"
#include "ComponentTypes.h"
#include "Calculator.h"

ApplicationManager::ApplicationManager(sf::VideoMode mode, std::string windowTitle, sf::Uint32 style) :
    window(mode, windowTitle, style) {
    // Set config values
    set(1600, 900);

    // Intialize Grid
    grid = Grid(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Set initialize placing type to wire
    placingComponentType = &WIRE;

    // Limit fps to 60
    window.setFramerateLimit(60);

	// Initialize textures
	sf::Texture resistor,
				vSrc,
                error;

    resistor.loadFromFile("assets/resistor.png");
    vSrc.loadFromFile("assets/vsrc.png");
    error.loadFromFile("assets/error.png");

	textures.push_back(resistor);
	textures.push_back(vSrc);
    textures.push_back(error);
};

void ApplicationManager::update() {
    // Clear the window and fill the background with BACKGROUND_COLOR
	window.clear(BACKGROUND_COLOR);

    // Poll for events
	sf::Event event;
	while (window.pollEvent(event)) {
		handleEvent(event);
	}

    // Draw the state
	draw();
	window.display();
}

////////////////////////
// Management Methods //
////////////////////////

void ApplicationManager::handleEvent(sf::Event event) {
    // Direct the events to the proper handler method
	switch (event.type) {
        case sf::Event::KeyPressed:
            handleKeypress(event.key);
            break;
        case sf::Event::TextEntered:
            handleTextEntered(event.text);
            break;
		case sf::Event::MouseButtonPressed:
			handleMousePressed(event.mouseButton);
			break;
        case sf::Event::Resized:
            handleResized(event.size);
            break;
		case sf::Event::Closed:
            window.close();
			break;
	}
}

void ApplicationManager::handleKeypress(sf::Event::KeyEvent event) {
    // Extract the keyboard id from the event
    sf::Keyboard::Key key = event.code;

    switch (key) {
        case sf::Keyboard::Escape:
            // Close the window
            window.close();
            break;
        case sf::Keyboard::Up:
        case sf::Keyboard::Down:
            // Shift the currently selected component type
            shift(placingComponentType, key);
            break;
        case sf::Keyboard::Space:
            // Toggle between PLACING and SELECTING mode
            if (mode == PLACING)
                mode = SELECTING;
            else if (mode == SELECTING || mode == SELECTED)
                mode = PLACING;
            selectedComponent = nullptr;
            break;
        case sf::Keyboard::C:
            // Clear all components
            grid.clearComponents();
            recalculate();
            break;
        case sf::Keyboard::S:
            // Enter TYPING mode, only if the component is not a wire
            if (mode == SELECTED && selectedComponent->type != &WIRE)
                mode = TYPING;
            break;
        case sf::Keyboard::D:
            // Delete the selected components
            if (mode == SELECTED) {
                grid.removeComponent(selectedComponent);
                selectedComponent = nullptr;
                recalculate();
            }
            break;
    }
}

void ApplicationManager::handleTextEntered(sf::Event::TextEvent event) {
    if (mode == TYPING) {
        // Get the key pressed from the event
        char ch = char(event.unicode);

        if (ch == 8) {
            // Backspace. Delete the last character
            input = input.substr(0, input.size() - 1);
        } else if (ch != 13 && ch != 10 && ch != 's') {
            // Adds input as long as the characters isn't a new line or 's'. 
            // This is because the key to activate typing mode is 's', however
            // every key press sends both a text entered event and a keyboard
            // press event. We have to ignore 's' inputs or else every input
            // would start with an 's'.
            input += ch;
        } else if (ch != 's') {
            // Only option is a newline, indicates user is done entering text
            setSelectedComponentValue(input);
            input = "";
            mode = SELECTED;
        }
    }
}

void ApplicationManager::handleMousePressed(sf::Event::MouseButtonEvent event) {
	if ((mode == PLACING || mode == PLACING_COMPONENT) && event.button == sf::Mouse::Left) {
        // Place a component

        // Get nearest grid spot, this will be the position that the component
        // is placed.
		GridSpot* spot = nullptr;
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		grid.getNearestSpot(mousePos, spot);

		if (mode == PLACING && spot != nullptr) {
            // Mode is placing; create a new component
			placingComponent = new Component(placingComponentType);
			placingComponent->positive = spot;
            mode = PLACING_COMPONENT;
		} else if (mode == PLACING_COMPONENT && spot != nullptr) {
            // A component is already in the process of being placed; complete
            // it
			placingComponent->negative = spot;

			grid.addComponent(placingComponent);
			spot->components.push_back(placingComponent);
            placingComponent->positive->components.push_back(placingComponent);

            mode = PLACING;

            // The state has changed, so the component values must be updated
            recalculate();
		}

    } else if ((mode == SELECTING || mode == SELECTED) && event.button == sf::Mouse::Right) {
        // Select a component, if applicable
        Component* component;
        if (grid.getComponentUnderPosition({ event.x, event.y }, component) && 
            component->type != &WIRE) {
            selectedComponent = component;
            mode = SELECTED;
        }
    }
}

void ApplicationManager::handleResized(sf::Event::SizeEvent event) {
    // Set SFML window view
    window.setView(sf::View(sf::FloatRect(0, 0, event.width, event.height)));

    // Recalculate Config values
    set(event.width, event.height);
    std::cout << "WIDTH: " << SCREEN_WIDTH << std::endl;

    // Redraw grid
    grid = Grid(event.width, event.height);
    placingComponent = nullptr;
    selectedComponent = nullptr;
}

///////////////////////
// Rendering methods //
///////////////////////

void ApplicationManager::draw() {
    // Draw the GUI
	drawGui();

    // Get the component under the mouse. Only used for determining colors of
    // drawn components.
    Component* comp;
    sf::Vector2i mousePos(sf::Mouse::getPosition(window));
    grid.getComponentUnderPosition(mousePos, comp);
    sf::Color color;

    // Loop over all component and draw them with the appropriate color
	for (Component* component : grid.getComponents()) {
        if ((mode == SELECTING || mode == SELECTED) && component == selectedComponent)
            color = COMPONENT_SELECTED_COLOR;
        else if ((mode == SELECTING || mode == SELECTED) && component == comp
                 && component->type != &WIRE)
            color = COMPONENT_HOVER_COLOR;
        else
            color = COMPONENT_COLOR;
        
        drawComponent(component, color);
	}

    // If the user is in the middle of placing a component, draw a "dummy"
    // component to allow the user to visualize what they're placing.
    if (mode == PLACING_COMPONENT) {
        GridSpot* spot;
        if (grid.getNearestSpot(sf::Mouse::getPosition(window), spot)) {
            placingComponent->negative = spot;
            drawComponent(placingComponent);
        }
    }
}

void ApplicationManager::drawGui() {
	// Draw debug objects. TODO: Remove
	if (false) {
		// Draw grid spots
		for (auto row : grid.getSpots()) {
            for (GridSpot* spot : row) {
                sf::CircleShape spotCircle(3.0f);
                spotCircle.setFillColor(sf::Color::Red);
                spotCircle.setPosition(float(spot->x - 3), float(spot->y - 3));
                window.draw(spotCircle);
            }
		}
	}

    // Draw left panel items
    drawTitlePanel();
	drawComponentPanel();
    drawInstructionPanel();
    drawInfoPanel();

    // Draw border around grid
    drawRectangleHollow(
        { float(GRID_LEFT_OFFSET), float(GRID_TOP_OFFSET) },
        { float(SCREEN_WIDTH - GRID_RIGHT_OFFSET), float(GRID_TOP_OFFSET) },
        { float(SCREEN_WIDTH - GRID_RIGHT_OFFSET), float(SCREEN_HEIGHT - GRID_BOTTOM_OFFSET) },
        { float(GRID_LEFT_OFFSET), float(SCREEN_HEIGHT - GRID_BOTTOM_OFFSET) },
        BORDER_COLOR
    );
}

void ApplicationManager::drawTitlePanel() {
    // Draw panel outline
    drawRectangleHollow(PANEL_TITLE_1, PANEL_TITLE_2, PANEL_TITLE_3, 
                        PANEL_TITLE_4, BORDER_COLOR);

    // Draw text
    sf::Text title;
    title.setFont(DEFAULT_FONT);
    title.setCharacterSize(DEFAULT_FONT_SIZE + 6);
    title.setString("Circuit Simulator");
    title.setFillColor(sf::Color::White);
    title.setPosition(float(GUI_X_PADDING + 15), float(GUI_Y_PADDING + 15));
    title.setStyle(sf::Text::Style::Bold);

    window.draw(title);
}

void ApplicationManager::drawComponentPanel() {
    sf::Text text;
    std::string name;

    // The component types will be iterated over, so keep track of the current
    // position and increment every time one is drawn.
    int xPos = GUI_X_PADDING + 15,
        yPos = GUI_Y_PADDING * 2 + 80;

    // Construct array to iterate over
    const ComponentType* types[3] = { &WIRE,
                                      &RESISTOR,
                                      &VSRC };

    // Draw panel outline
    drawRectangleHollow(PANEL_COMP_1, PANEL_COMP_2, PANEL_COMP_3, PANEL_COMP_4,
                        BORDER_COLOR);

    // Draw panel title
	sf::Text title;
	title.setFont(DEFAULT_FONT);
	title.setCharacterSize(DEFAULT_FONT_SIZE + 2);
	title.setString("Selected Type:");
    title.setFillColor(sf::Color::White);
	title.setPosition(float(xPos), float(yPos));

	window.draw(title);

	yPos += 35;


    // Iterate of the ComponentTypes and display each one
    for (const ComponentType* type : types) {
        name = type->getName();

        text.setFont(DEFAULT_FONT);
        text.setCharacterSize(DEFAULT_FONT_SIZE);
        text.setString(name);
        text.setFillColor(sf::Color::White);
        text.setPosition(float(xPos), float(yPos));

        // If this component type is currently selected, draw a box below it
        // to emphasize this to the user
        if ((mode == PLACING || mode == PLACING_COMPONENT) && 
            type == placingComponentType) {
            text.setFillColor({ 40, 40, 40 });
            sf::RectangleShape rect({ 265, 20 });
            rect.setFillColor({ 225, 225, 225 });
            rect.setPosition(float(xPos - 3), float(yPos + 1));
            window.draw(rect);
        }

        window.draw(text);

        yPos += 20;
    }
}

void ApplicationManager::drawInstructionPanel() {
    sf::Text title,
             info;
    std::string infoStr;

    // Draw panel outline
    drawRectangleHollow(PANEL_INSTRUCT_1, PANEL_INSTRUCT_2, 
                        PANEL_INSTRUCT_3, PANEL_INSTRUCT_4, BORDER_COLOR);

    // Draw panel title
    std::string titleStr = "Mode: ";
    title.setFont(DEFAULT_FONT);
    title.setCharacterSize(DEFAULT_FONT_SIZE);
    title.setFillColor(sf::Color::White);
    title.setPosition(float(GUI_X_PADDING + 15), float(GUI_Y_PADDING * 3 + 215));

    // Draw a different title string depending on the mode
    switch (mode) {
        case PLACING:
        case PLACING_COMPONENT:
            titleStr += "Placing";
            break;
        case SELECTING:
            titleStr += "Component Select";
            break;
        case SELECTED:
            titleStr += "Component Selected";
            break;
        case TYPING:
            titleStr += "Setting Value";
            break;
    }

    title.setString(titleStr);
    window.draw(title);

    // Display a basic description of the current mode
    info.setFont(DEFAULT_FONT);
    info.setCharacterSize(DEFAULT_FONT_SIZE - 2);
    info.setFillColor(sf::Color::White);
    info.setPosition(float(GUI_X_PADDING + 15), float(GUI_Y_PADDING * 3 + 250));

    // Draw different information text depending on the mode
    if (mode == PLACING || mode == PLACING_COMPONENT) {
        infoStr = "Place components on the\ngrid.";
    } else if (mode == SELECTING) {
        infoStr = "Select a component in\norder to view its info or\nchange "
                  "its properties.";
    } else if (mode == SELECTED) {
        infoStr = "Select a component in\norder to view its info or\nchange "
                  "its properties.";
    } else if (mode == TYPING) {
        infoStr = "Enter the component\nvalue from your keyboard.\nStart "
                  "typing, and your\ninput will be shown below.\n\nCurrent "
                  "Input:\n   " + input;
    }

    info.setString(infoStr);
    window.draw(info);

    // Display error message if there was a calculation error
    if (error) {
        sf::Texture tex = textures.at(2);
        sf::Vector2f spritePos1(PANEL_INSTRUCT_4 + sf::Vector2f{ 40.0, -25.0 }),
                     spritePos2(PANEL_INSTRUCT_3 + sf::Vector2f{ -40.0, -25.0 });
        sf::Sprite error1, 
                   error2;
        sf::Text warning;

        error1.setOrigin(float(tex.getSize().x / 2.0), float(tex.getSize().y / 2.0));
        error1.setTexture(tex);
        error1.setPosition(spritePos1);
        error1.setScale(float(1.0 / 6.5), float(1.0 / 6.5));

        error2 = sf::Sprite(error1);
        error2.setPosition(spritePos2);

        warning.setFont(DEFAULT_FONT);
        warning.setCharacterSize(DEFAULT_FONT_SIZE - 3);
        warning.setPosition(spritePos1 + sf::Vector2f{ 30.0, -20.0 });
        warning.setString("Error calculating\n circuit values");

        warning.setFillColor(WARNING_COLOR);
        error1.setColor(WARNING_COLOR);
        error2.setColor(WARNING_COLOR);

        window.draw(error1);
        window.draw(error2);
        window.draw(warning);
    }
}

void ApplicationManager::drawInfoPanel() {
    // Draw panel outline
    drawRectangleHollow(PANEL_INFO_1, PANEL_INFO_2, PANEL_INFO_3, 
                        PANEL_INFO_4, BORDER_COLOR);

    if (selectedComponent != nullptr && selectedComponent->type != &WIRE) {
        // Display component information, including the voltage and current
        // across it, and its value if it isn't a wire.
        sf::Text title,
                 voltageDrop,
                 ampsThrough;
        std::string titleStr,
                    voltageDropStr,
                    ampsThroughStr;
        std::stringstream ss;

        titleStr = "Selected Component Type:\n\t" + selectedComponent->type->getName();

        ss << "Voltage Difference:\n\t" << std::setprecision(3) << std::fixed 
           << selectedComponent->voltageDrop << " V";

        voltageDropStr = ss.str();
        // Clear the string stream
        ss.str(std::string());

        ss << "Current Through:\n\t" << std::setprecision(3) << std::fixed 
           << selectedComponent->currentThrough << " A";
        ampsThroughStr = ss.str();

        // Clear the string stream
        ss.str(std::string());

        title.setFont(DEFAULT_FONT);
        title.setCharacterSize(DEFAULT_FONT_SIZE - 2);
        title.setFillColor(sf::Color::White);
        title.setString(titleStr);
        title.setPosition(float(GUI_X_PADDING + 15), 
                          float(GUI_Y_PADDING * 4 + 540));

        voltageDrop.setFont(DEFAULT_FONT);
        voltageDrop.setCharacterSize(DEFAULT_FONT_SIZE - 2);
        voltageDrop.setFillColor(sf::Color::White);
        voltageDrop.setString(voltageDropStr);
        voltageDrop.setPosition(float(GUI_X_PADDING + 15), 
                                float(GUI_Y_PADDING * 4 + 590));

        ampsThrough.setFont(DEFAULT_FONT);
        ampsThrough.setCharacterSize(DEFAULT_FONT_SIZE - 2);
        ampsThrough.setFillColor(sf::Color::White);
        ampsThrough.setString(ampsThroughStr);
        ampsThrough.setPosition(float(GUI_X_PADDING + 15), 
                                float(GUI_Y_PADDING * 4 + 640));

        window.draw(title);
        window.draw(voltageDrop);
        window.draw(ampsThrough);

        // Check if the component type is a wire so the value part isn't
        // drawn unnecessarily
        if (selectedComponent->type != &WIRE) {
            ss << "Value:\n\t" << std::setprecision(3) << std::fixed << selectedComponent->value;

            if (selectedComponent->type == &RESISTOR)
                ss << " Ohms";
            else
                ss << " V";

            sf::Text value;
            value.setFont(DEFAULT_FONT);
            value.setCharacterSize(DEFAULT_FONT_SIZE - 2);
            value.setFillColor(sf::Color::White);
            value.setString(ss.str());
            value.setPosition(float(GUI_X_PADDING + 15), float(GUI_Y_PADDING * 4 + 690));

            window.draw(value);
        }
    } else {
        // Display simple placeholder message
        sf::Text text;
        text.setFont(DEFAULT_FONT);
        text.setCharacterSize(DEFAULT_FONT_SIZE);
        text.setString("No Component Selected");
        text.setFillColor(sf::Color::White);
        text.setPosition(float(GUI_X_PADDING + 30), float(GUI_Y_PADDING * 4 + 610));

        window.draw(text);
    }
}

void ApplicationManager::drawComponent(Component* component, sf::Color color) {
	sf::Vector2f posSpot = component->positive->getVector();
	sf::Vector2f negSpot = component->negative->getVector();
	sf::CircleShape c1(5.0f),
                    c2;
    const ComponentType* type;
    float length,
          radians;
    sf::Vector2f offset;

    // Draw component endpoints
    c1.setOrigin({ 5.0f, 5.0f });
	c1.setPosition(posSpot);
    c1.setFillColor(color);
    c1.setOutlineColor({ 120, 120, 120 });
	c1.setOutlineThickness(1.0f);

    c2 = sf::CircleShape(c1);
	c2.setPosition(negSpot);

	window.draw(c1);
	window.draw(c2);

    // Draw actual component
    type = component->type;

    // Get length and angle of the component.
    length = float(sqrt(pow(posSpot.x - negSpot.x, 2.0) + pow(posSpot.y - negSpot.y, 2.0)));
    radians = PI + atan2(posSpot.y - negSpot.y, posSpot.x - negSpot.x);

    // The rectangle objects like to be off by one pixel at all times. This
    // vector is added to their position to correct that.
    offset = { sin(radians), -cos(radians) };

    if (type == &WIRE) {
        sf::RectangleShape rect(sf::Vector2f(length, 2.0f));
        rect.setFillColor(color);
        rect.setRotation(radians * 180.0f / PI);
        rect.setPosition(posSpot + offset);

        window.draw(rect);
    } else {
        sf::Texture tex;
        sf::Vector2f spritePos;
        sf::Sprite sprite;
        sf::RectangleShape rect1,
                           rect2;

        // Draw different textures depending on component types (with slightly
        // different positions)
        if (type == &RESISTOR) {
            tex = textures.at(0);
            spritePos = { posSpot.x - (posSpot.x - negSpot.x) / 2.0f + sin(radians) * float(tex.getSize().x) / 4.0f,
                          posSpot.y - (posSpot.y - negSpot.y) / 2.0f - cos(radians) * float(tex.getSize().y) / 2.0f };
        } else if (type == &VSRC) {
            tex = textures.at(1);
            spritePos = { posSpot.x - (posSpot.x - negSpot.x) / 2.0f + sin(radians) * float(tex.getSize().x) / 2.0f,
                          posSpot.y - (posSpot.y - negSpot.y) / 2.0f - cos(radians) * float(tex.getSize().y) / 2.0f };
        }

        rect1 = sf::RectangleShape({ 0.5f * (length - float(tex.getSize().x)), 2.0f });
        rect1.setFillColor(color);
        rect1.setRotation(radians * 180.0f / PI);
        rect1.setPosition(posSpot + offset);

        rect2 = sf::RectangleShape(rect1);
        rect2.setRotation((radians + PI) * 180.0f / PI);
        rect2.setPosition(negSpot + -offset);

        sprite.setOrigin({ float(tex.getSize().x) / 2.0f, 0.0f });
        sprite.setTexture(tex);
        sprite.setPosition(spritePos);
        sprite.setRotation(radians * (180.0f / PI));
        sprite.setColor(color);

        window.draw(rect1);
        window.draw(rect2);
        window.draw(sprite);
    }
}

void ApplicationManager::drawRectangleHollow(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4, sf::Color color) {
    drawLine(p1, p2, color);
    drawLine(p2, p3, color);
    drawLine(p3, p4, color);
    drawLine(p4, p1, color);
}

void ApplicationManager::drawLine(sf::Vector2f p1, sf::Vector2f p2, sf::Color color) {
    // Convert vectors to vertices
    sf::Vertex v1(p1),
               v2(p2);

    v1.color = color;
    v2.color = color;

    const sf::Vertex vertices[2] = { v1, v2 };
    window.draw(vertices, 2, sf::PrimitiveType::Lines);
}

////////////////////
// Helper Methods //
////////////////////

void ApplicationManager::setSelectedComponentValue(std::string input) {
    double val;
    std::string prefix;
    std::regex reg("^([\\d\\.]+) ?(n|u|m|k|M|G)?$");
    std::smatch match;

    // Match the input string to the regex validation
    if (std::regex_search(input, match, reg) && match.size() > 1 && selectedComponent != nullptr) {
        val = std::stod(match.str(1));

        if (match.size() > 2) {
            prefix = match.str(2);

            // Take units into account when setting the value. 
            if (prefix == "n")
                val *= pow(10.0, -9.0);
            else if (prefix == "u")
                val *= pow(10.0, -6.0);
            else if (prefix == "m")
                val *= pow(10.0, -3.0);
            else if (prefix == "k")
                val *= pow(10.0, 3.0);
            else if (prefix == "M")
                val *= pow(10.0, 6.0);
            else if (prefix == "G")
                val *= pow(10.0, 9.0);
        }

        // Update component value
        selectedComponent->value = val;
    }
}

void ApplicationManager::recalculate() {
    // Attempt to calculate all voltage and current values for all components
    // on the grid. Catch and exception and, no matter the exception, display
    // a warning to the user.
    try {
        Calculator::calculate(grid.getSpots(), grid.getComponents());
        error = false;
    } catch (...) {
        error = true;
    }
}