#include <iomanip>
#include <string>
#include <cmath>
#include <regex>
#include <sstream>
#include "ApplicationManager.h"
#include "ComponentTypes.h"
#include "Calculator.h"

ApplicationManager::ApplicationManager(sf::VideoMode mode, std::string windowTitle, sf::Uint32 style) :
    window(mode, windowTitle, style) {

    grid = Grid();
    placingComponentType = &WIRE;

    // Limit fps to 60
    window.setFramerateLimit(60);

	// Initialize textures
	sf::Texture resistor,
				vSrc,
                error;

    resistor.loadFromFile("resistor.png");
    vSrc.loadFromFile("vsrc.png");
    error.loadFromFile("error.png");

	textures.push_back(resistor);
	textures.push_back(vSrc);
    textures.push_back(error);
};

void ApplicationManager::update() {
	window.clear(BACKGROUND_COLOR);

	sf::Event event;
	while (window.pollEvent(event)) {
		handleEvent(event);
	}

	draw();
	window.display();
}

////////////////////////
// Management Methods //
////////////////////////

void ApplicationManager::handleEvent(sf::Event event) {
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
		case sf::Event::Closed:
            window.close();
			break;
	}
}

void ApplicationManager::handleKeypress(sf::Event::KeyEvent event) {
    sf::Keyboard::Key key = event.code;

    switch (key) {
        case sf::Keyboard::Escape:
            window.close();
            break;
        case sf::Keyboard::Up:
        case sf::Keyboard::Down:
            shift(placingComponentType, key);
            break;
        case sf::Keyboard::Space:
            if (mode == PLACING)
                mode = SELECTING;
            else if (mode == SELECTING || mode == SELECTED)
                mode = PLACING;
            selectedComponent = nullptr;
            break;
        case sf::Keyboard::C:
            grid.clearComponents();
            break;
        case sf::Keyboard::S:
            if (mode == SELECTED && selectedComponent->type != &WIRE)
                mode = TYPING;
            break;
        case sf::Keyboard::D:
            grid.removeComponent(selectedComponent);
            selectedComponent = nullptr;
            recalculate();
            break;
    }

    recalculate();
}

void ApplicationManager::handleTextEntered(sf::Event::TextEvent event) {
    if (mode == TYPING) {
        char ch = char(event.unicode);

        if (ch == 8) {
            input = input.substr(0, input.size() - 1);
        } else if (ch != 13 && ch != 's') {
            input += ch;
        } else if (ch != 's') {
            setSelectedComponentValue(input);
            input = "";
            mode = SELECTED;
        }
    }
}

void ApplicationManager::handleMousePressed(sf::Event::MouseButtonEvent event) {
	if ((mode == PLACING || mode == PLACING_COMPONENT) && event.button == sf::Mouse::Left) {
		GridSpot* spot = nullptr;
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		grid.getNearestSpot(mousePos, spot);

		if (mode == PLACING && spot != nullptr) {
			placingComponent = new Component(placingComponentType);
			placingComponent->positive = spot;
            mode = PLACING_COMPONENT;
		} else if (mode == PLACING_COMPONENT && spot != nullptr) {
			placingComponent->negative = spot;

			grid.addComponent(placingComponent);
			spot->components.push_back(placingComponent);
            placingComponent->positive->components.push_back(placingComponent);

            mode = PLACING;
		}
    } else if ((mode == SELECTING || mode == SELECTED) && event.button == sf::Mouse::Right) {
        Component* component;
        if (grid.getComponentUnderPosition({ event.x, event.y }, component)) {
            selectedComponent = component;
            mode = SELECTED;
        }
    }

    recalculate();
}


///////////////////////
// Rendering methods //
///////////////////////

// Main rendering methods
void ApplicationManager::draw() {
	drawGui();

    Component* comp;
    sf::Vector2i mousePos(sf::Mouse::getPosition(window));
    grid.getComponentUnderPosition(mousePos, comp);
    sf::Color color;

	for (Component* component : grid.getComponents()) {
        if ((mode == SELECTING || mode == SELECTED) && component == selectedComponent)
            color = COMPONENT_SELECTED_COLOR;
        else if ((mode == SELECTING || mode == SELECTED) && component == comp)
            color = COMPONENT_HOVER_COLOR;
        else
            color = COMPONENT_COLOR;
        
        drawComponent(component, color);
	}

    if (mode == PLACING_COMPONENT) {
        GridSpot* spot;
        if (grid.getNearestSpot(sf::Mouse::getPosition(window), spot)) {
            placingComponent->negative = spot;
            drawComponent(placingComponent);
        }
    }
}

void ApplicationManager::drawGui() {
	// Draw debug objects
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
    drawRectangleHollow(window,
        { float(GRID_LEFT_OFFSET), float(GRID_TOP_OFFSET) },
        { float(SCREEN_WIDTH - GRID_RIGHT_OFFSET), float(GRID_TOP_OFFSET) },
        { float(SCREEN_WIDTH - GRID_RIGHT_OFFSET), float(SCREEN_HEIGHT - GRID_BOTTOM_OFFSET) },
        { float(GRID_LEFT_OFFSET), float(SCREEN_HEIGHT - GRID_BOTTOM_OFFSET) },
        BORDER_COLOR);
}

void ApplicationManager::drawTitlePanel() {
    // Draw panel outline
    drawRectangleHollow(window, PANEL_TITLE_1, PANEL_TITLE_2, PANEL_TITLE_3, 
                        PANEL_TITLE_4, BORDER_COLOR);

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
    int xPos = GUI_X_PADDING + 15,
        yPos = GUI_Y_PADDING * 2 + 80;

    // Draw panel outline
    drawRectangleHollow(window, PANEL_COMP_1, PANEL_COMP_2, PANEL_COMP_3, 
                        PANEL_COMP_4, BORDER_COLOR);

	sf::Text title;
	title.setFont(DEFAULT_FONT);
	title.setCharacterSize(DEFAULT_FONT_SIZE + 2);
	title.setString("Selected Component:");
    title.setFillColor(sf::Color::White);
	title.setPosition(float(xPos), float(yPos));

	window.draw(title);

	yPos += 35;

    // Construct array to iterate over
    const ComponentType* types[3] = { &WIRE, 
                                      &RESISTOR, 
                                      &VSRC };

    for (const ComponentType* type : types) {
        std::string name = type->getName();

        sf::Text text;
        text.setFont(DEFAULT_FONT);
        text.setCharacterSize(DEFAULT_FONT_SIZE);
        text.setString(name);
        text.setFillColor(sf::Color::White);
        text.setPosition(float(xPos), float(yPos));

        if (type == placingComponentType) {
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
    // Draw panel outline
    drawRectangleHollow(window, PANEL_INSTRUCT_1, PANEL_INSTRUCT_2, 
                        PANEL_INSTRUCT_3, PANEL_INSTRUCT_4, BORDER_COLOR);

    std::string titleStr = "Mode: ";
    sf::Text title;
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
    }

    title.setString(titleStr);
    window.draw(title);

    // Display help information, such as available key presses
    sf::Text info;
    info.setFont(DEFAULT_FONT);
    info.setCharacterSize(DEFAULT_FONT_SIZE - 2);
    info.setFillColor(sf::Color::White);
    info.setPosition(float(GUI_X_PADDING + 15), float(GUI_Y_PADDING * 3 + 250));

    std::string infoStr;

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
        sf::Vector2f spritePos1(PANEL_INSTRUCT_4 + sf::Vector2f{ 40.0, -25.0 });
        sf::Vector2f spritePos2(PANEL_INSTRUCT_3 + sf::Vector2f{ -40.0, -25.0 });
        sf::Sprite error1, error2;
        error1.setOrigin(float(tex.getSize().x / 2.0), float(tex.getSize().y / 2.0));
        error1.setTexture(tex);
        error1.setPosition(spritePos1);
        error1.setScale(float(1.0 / 6.5), float(1.0 / 6.5));
        error2.setOrigin(float(tex.getSize().x / 2.0), float(tex.getSize().y / 2.0));
        error2.setTexture(tex);
        error2.setPosition(spritePos2);
        error2.setScale(float(1.0 / 6.5), float(1.0 / 6.5));

        sf::Text warning;
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
    drawRectangleHollow(window, PANEL_INFO_1, PANEL_INFO_2, PANEL_INFO_3, 
                        PANEL_INFO_4, BORDER_COLOR);

    if (selectedComponent != nullptr) {
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

	sf::CircleShape c1(5.0f);
    c1.setOrigin({ 5.0f, 5.0f });
	c1.setPosition(posSpot);
    c1.setFillColor(color);
    c1.setOutlineColor({ 120, 120, 120 });
	c1.setOutlineThickness(1.0f);

	sf::CircleShape c2(c1);
	c2.setPosition(negSpot);

	window.draw(c1);
	window.draw(c2);

    // Draw actual component
    const ComponentType* type = component->type;

    float length = float(sqrt(pow(posSpot.x - negSpot.x, 2.0) + pow(posSpot.y - negSpot.y, 2.0)));
    float radians = PI + atan2(posSpot.y - negSpot.y, posSpot.x - negSpot.x);
    sf::Vector2f offset = { sin(radians), -cos(radians) };

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

        if (type == &RESISTOR) {
            tex = textures.at(0);
            spritePos = { posSpot.x - (posSpot.x - negSpot.x) / 2.0f + sin(radians) * float(tex.getSize().x) / 4.0f,
                          posSpot.y - (posSpot.y - negSpot.y) / 2.0f - cos(radians) * float(tex.getSize().y) / 2.0f };
        } else if (type == &VSRC) {
            tex = textures.at(1);
            spritePos = { posSpot.x - (posSpot.x - negSpot.x) / 2.0f + sin(radians) * float(tex.getSize().x) / 2.0f,
                          posSpot.y - (posSpot.y - negSpot.y) / 2.0f - cos(radians) * float(tex.getSize().y) / 2.0f };
        }

        sf::RectangleShape rect1({ 0.5f * (length - float(tex.getSize().x)), 2.0f });
        rect1.setFillColor(color);
        rect1.setRotation(radians * 180.0f / PI);
        rect1.setPosition(posSpot + offset);

        sf::RectangleShape rect2(rect1);
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

void ApplicationManager::setSelectedComponentValue(std::string input) {
    double val;
    std::string prefix;
    std::regex reg("^([\\d\\.]+) ?(n|m|k|M|G)?$");
    std::smatch match;

    if (std::regex_search(input, match, reg) && match.size() > 1 && selectedComponent != nullptr) {
        val = std::stod(match.str(1));

        if (match.size() > 2) {
            prefix = match.str(2);

            if (prefix == "n")
                val *= pow(10.0, -9.0);
            else if (prefix == "m")
                val *= pow(10.0, -3.0);
            else if (prefix == "k")
                val *= pow(10.0, 3.0);
            else if (prefix == "M")
                val *= pow(10.0, 6.0);
            else if (prefix == "G")
                val *= pow(10.0, 9.0);
        }

        selectedComponent->value = val;
    }
}

void ApplicationManager::recalculate() {
    try {
        Calculator::calculate(grid.getSpots(), grid.getComponents());
        error = false;
    } catch (...) {
        error = true;
    }
}

void ApplicationManager::drawRectangleHollow(sf::RenderWindow& window, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4, sf::Color color) {
    drawLine(window, p1, p2, color);
    drawLine(window, p2, p3, color);
    drawLine(window, p3, p4, color);
    drawLine(window, p4, p1, color);
}

void ApplicationManager::drawLine(sf::RenderWindow& window, sf::Vector2f p1, sf::Vector2f p2, sf::Color color) {
    sf::Vertex v1(p1),
               v2(p2);

    v1.color = color;
    v2.color = color;

    const sf::Vertex vertices[2] = { p1, p2 };
    window.draw(vertices, 2, sf::PrimitiveType::Lines);
}