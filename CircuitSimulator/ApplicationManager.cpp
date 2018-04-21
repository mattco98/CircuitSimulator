#include <iostream>
#include <string>
#include <cmath>
#include <regex>
#include "ApplicationManager.h"
#include "ComponentTypes.h"
#include "GuiHelper.h"
#include "Calculator.h"

// TODO: Debug
using std::cout;
using std::endl;

ApplicationManager::ApplicationManager(sf::VideoMode mode, std::string windowTitle, sf::Uint32 style) :
	// Initialize window and grid
	window(mode, windowTitle, style),
	grid(),
	placingComponentType(&ComponentTypes::WIRE) {

    // Limit fps to 60
    window.setFramerateLimit(60);

	// Initialize textures
	sf::Texture resistor,
				vSrc;

    resistor.loadFromFile("resistor.png");
    vSrc.loadFromFile("vsrc.png");

	textures.push_back(resistor);
	textures.push_back(vSrc);
};

void ApplicationManager::update() {
    std::vector< std::vector<GridSpot*> > spots = grid.getSpots();

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
			handleClosed();
			break;
	}
}

void ApplicationManager::handleTextEntered(sf::Event::TextEvent event) {
    if (mode == TYPING) {
        char ch = char(event.unicode);

        if (ch == 't') {
            return;
        } else if (ch == 8) {
            input = input.substr(0, input.size() - 1);
        } else if (ch != 13) {
            input += ch;
        } else {
            setSelectedComponentValue(input);
            input = "";
            mode = SELECTED;
        }
    }
}

void ApplicationManager::handleKeypress(sf::Event::KeyEvent event) {
	unsigned short key = event.code;
	int i;

	switch (key) {
		case sf::Keyboard::Escape:
			handleClosed();
			break;
		case sf::Keyboard::Up:
			i = placingComponentType->getValue() - 1;

			if (i < 0)
				i = ComponentTypes::NUM_TYPES - 1;

			placingComponentType = ComponentTypes::getType(i);
			break;
		case sf::Keyboard::Down:
			i = placingComponentType->getValue() + 1;

			if (i > ComponentTypes::NUM_TYPES - 1)
				i = 0;

            placingComponentType = ComponentTypes::getType(i);
			break;
        case sf::Keyboard::Space:
            if (mode == PLACING || mode == SELECTED) {
                mode = SELECTING;
                selectedComponent = nullptr;
            }
            break;
        case sf::Keyboard::P:
            if (mode == SELECTING || mode == SELECTED) {
                mode = PLACING;
                selectedComponent = nullptr;
            }
            break;
		case sf::Keyboard::C:
			grid.clearComponents();
			grid.clearComponents();
			break;
        case sf::Keyboard::T:
            if (mode == SELECTED) {
                mode = TYPING;
                
            }
            break;
        case sf::Keyboard::K:
            Calculator::calculate(grid.getSpots(), grid.getComponents());
            break;
	}
}

void ApplicationManager::handleMousePressed(sf::Event::MouseButtonEvent event) {
	if (event.button == sf::Mouse::Left) {
		GridSpot* spot = nullptr;
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		grid.getNearestSpot(mousePos, &spot);

		if (mode == PLACING && spot != nullptr) {
			placingComponent = new Component(placingComponentType);
			placingComponent->setPositive(&spot);
            mode = PLACING_COMPONENT;
		} else if (mode == PLACING_COMPONENT && spot != nullptr) {
			placingComponent->setNegative(&spot);

			grid.addComponent(placingComponent);
			spot->components.push_back(placingComponent);
            placingComponent->getPositive()->components.push_back(placingComponent);

            mode = PLACING;
		}
    } else if (event.button == sf::Mouse::Right && (mode == SELECTING || mode == SELECTED)) {
        Component* component;
        if (grid.getComponentUnderPosition({ event.x, event.y }, component)) {
            selectedComponent = component;
            mode = SELECTED;
        }
    }
}

void ApplicationManager::handleResized(sf::Event event) {
	// TODO
	// event.size.width, event.size.height
}

void ApplicationManager::handleClosed() {
	// TODO: Confirmation dialog, etc
	window.close();
}


///////////////////////
// Rendering methods //
///////////////////////

// Main rendering methods
void ApplicationManager::draw() {
	drawGui();

	for (Component* component : grid.getComponents()) {
		drawComponent(component);
	}

    if (mode == PLACING_COMPONENT) {
        Component* hover = placingComponent;
        GridSpot* spot;
        if (grid.getNearestSpot(sf::Mouse::getPosition(window), &spot)) {
            hover->setNegative(&spot);
            drawComponent(hover);
        }
    }
}

//void ApplicationManager::transitionAlpha() {
//    // When deltaAlpha = 5.167, transition takes exactly 0.5 seconds at 60FPS
//    if (alphaTransition == 1) {
//        if (alpha < 255.0) {
//            alpha += 5.167;
//        } else {
//            alphaTransition = 0;
//        }
//        if (alpha > 255.0)
//            alpha = 255.0;
//    } else if (alphaTransition == 2) {
//        if (alpha > 100.0) {
//            alpha -= 5.167;
//        } else {
//            alphaTransition = 0;
//        }
//        if (alpha < 100.0)
//            alpha = 100;
//    }
//}

void ApplicationManager::drawGui() {
    bool debug = false;

	// Draw debug objects
	if (debug) {
		// Draw grid spots
		for (auto row : grid.getSpots()) {
            for (GridSpot* spot : row) {
                cout << "(" << spot->x << ", " << spot->y << ")\n";
                sf::CircleShape spotCircle(3.0f);
                spotCircle.setFillColor(GuiHelper::applyAlpha(sf::Color::Red, alpha));
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
    GuiHelper::drawRectangleHollow(window,
                                   { float(GRID_LEFT_OFFSET), float(GRID_TOP_OFFSET) },
                                   { float(SCREEN_WIDTH - GRID_RIGHT_OFFSET), float(GRID_TOP_OFFSET) },
                                   { float(SCREEN_WIDTH - GRID_RIGHT_OFFSET), float(SCREEN_HEIGHT - GRID_BOTTOM_OFFSET) },
                                   { float(GRID_LEFT_OFFSET), float(SCREEN_HEIGHT - GRID_BOTTOM_OFFSET) },
                                   GuiHelper::applyAlpha(BORDER_COLOR, alpha));

	// Draw dot on nearest grid spot
	GridSpot* nearestSpot;

	sf::Vector2i mousePos(sf::Mouse::getPosition(window));
	if (!grid.getNearestSpot(mousePos, &nearestSpot)) {
		return;
	}

    if (mode == PLACING) {
        sf::CircleShape mouseCircle(5.0f);
        mouseCircle.setFillColor(GuiHelper::applyAlpha(sf::Color(74, 77, 82), alpha));
        mouseCircle.setOutlineColor(sf::Color(47, 49, 54));
        mouseCircle.setOutlineThickness(1.0f);
        mouseCircle.setPosition(float(nearestSpot->x - 7), float(nearestSpot->y - 7));

        window.draw(mouseCircle);
    }
}

void ApplicationManager::drawTitlePanel() {
    // Draw panel outline
    GuiHelper::drawRectangleHollow(window, PANEL_TITLE_1, PANEL_TITLE_2, PANEL_TITLE_3, PANEL_TITLE_4,
                                   GuiHelper::applyAlpha(BORDER_COLOR, alpha));

    sf::Text title;
    title.setFont(DEFAULT_FONT);
    title.setCharacterSize(DEFAULT_FONT_SIZE + 6);
    title.setString("Circuit Simulator");
    title.setFillColor(GuiHelper::applyAlpha(sf::Color::White, alpha));
    title.setPosition(float(GUI_X_PADDING + 15), float(GUI_Y_PADDING + 15));
    title.setStyle(sf::Text::Style::Bold);

    window.draw(title);
}

void ApplicationManager::drawComponentPanel() {
	int xPos = GUI_X_PADDING + 15,
		yPos = GUI_Y_PADDING * 2 + 80,
		index = 0;

    // Draw panel outline
    GuiHelper::drawRectangleHollow(window, PANEL_COMP_1, PANEL_COMP_2, PANEL_COMP_3, PANEL_COMP_4,
            GuiHelper::applyAlpha(BORDER_COLOR, alpha));

	sf::Text title;
	title.setFont(DEFAULT_FONT);
	title.setCharacterSize(DEFAULT_FONT_SIZE + 2);
	title.setString("Selected Component:");
    title.setFillColor(GuiHelper::applyAlpha(sf::Color::White, alpha));
	title.setPosition(float(xPos), float(yPos));

	window.draw(title);

	yPos += 35;

	for (int i = 0; i < ComponentTypes::NUM_TYPES; i++) {
		std::string name = ComponentTypes::getType(i)->getName();

		sf::Text text;
		text.setFont(DEFAULT_FONT);
		text.setCharacterSize(DEFAULT_FONT_SIZE);
		text.setString(name);
        text.setFillColor(GuiHelper::applyAlpha(sf::Color::White, alpha));
		text.setPosition(float(xPos), float(yPos));

		if (i == placingComponentType->getValue()) {
			text.setFillColor(GuiHelper::applyAlpha(sf::Color(40, 40, 40), alpha));
			sf::RectangleShape rect(sf::Vector2f(265, 20));
			rect.setFillColor(GuiHelper::applyAlpha(sf::Color(225, 225, 225), alpha));
			rect.setPosition(sf::Vector2f(float(xPos - 3), float(yPos + 1)));
			window.draw(rect);
		}

		window.draw(text);

		yPos += 20;
		index++;
	}
}

void ApplicationManager::drawInstructionPanel() {
    // Draw panel outline
    GuiHelper::drawRectangleHollow(window, PANEL_INSTRUCT_1, PANEL_INSTRUCT_2, PANEL_INSTRUCT_3, PANEL_INSTRUCT_4,
                                   GuiHelper::applyAlpha(BORDER_COLOR, alpha));

    // Draw different text depending on mode

    // Title - display mode
    std::string titleStr = "Mode: ";
    sf::Text title;
    title.setFont(DEFAULT_FONT);
    title.setCharacterSize(DEFAULT_FONT_SIZE);
    title.setFillColor(GuiHelper::applyAlpha(sf::Color::White, alpha));
    title.setPosition(float(GUI_X_PADDING + 15), float(GUI_Y_PADDING * 3 + 215));

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
            titleStr += "Text Input";
    }

    title.setString(titleStr);
    window.draw(title);

    // Display help information, such as available key presses
    sf::Text info;
    info.setFont(DEFAULT_FONT);
    info.setCharacterSize(DEFAULT_FONT_SIZE - 2);
    info.setFillColor(GuiHelper::applyAlpha(sf::Color::White, alpha));
    info.setPosition(float(GUI_X_PADDING + 15), float(GUI_Y_PADDING * 3 + 250));

    // Draw different information text depending on the mode
    if (mode == PLACING || mode == PLACING_COMPONENT) {
        info.setString("In placing mode, you can\nplace different components\non the grid.\n\n"
                       "Available Actions:\n   Left click:\n\t  Place a component\n   Space:\n\t  Selection mode");
    } else if (mode == SELECTING) {
        info.setString("In selection mode, you can\nselect a component in\norder to view its "
                       "info or\nchange its properties.\n\n"
                       "Available Actions:\n   Right click:\n\t   Select the\n\t   component under the\n\t   cursor\n"
                       "   P:\n\t   Placing mode");
    } else if (mode == SELECTED) {
        info.setString("In selection mode, you can\nselect a component in\norder to view its "
                       "info or\nchange its properties.\n\n"
                       "Available Actions:\n   Right click:\n\t   Select the\n\t   component under the\n\t   cursor\n"
                       "   P:\n\t   Placing mode\n   T:\n\t   Text entry mode");
    } else if (mode == TYPING) {
        info.setString("In text entry mode, you\ncan enter the component\nvalue from your keyboard.\nStart typing, and your\ninput "
                       "will be shown below.\n\nCurrent Input:\n   " + input);
    }

    window.draw(info);
}

void ApplicationManager::drawInfoPanel() {
    // Draw panel outline
    GuiHelper::drawRectangleHollow(window, PANEL_INFO_1, PANEL_INFO_2, PANEL_INFO_3, PANEL_INFO_4,
                                   GuiHelper::applyAlpha(BORDER_COLOR, alpha));

    if (selectedComponent != nullptr) {
        sf::Text title,
            voltageDrop,
            ampsThrough;

        std::string titleStr,
            voltageDropStr,
            ampsThroughStr;

        titleStr = "Selected Component Type:\n\t" + selectedComponent->getType()->getName();
        voltageDropStr = "Voltage difference:\n\t" + std::to_string(selectedComponent->getVoltageDrop());
        ampsThroughStr = "Current through:\n\t" + std::to_string(selectedComponent->getAmpsThrough());

        title.setFont(DEFAULT_FONT);
        voltageDrop.setFont(DEFAULT_FONT);
        ampsThrough.setFont(DEFAULT_FONT);

        title.setCharacterSize(DEFAULT_FONT_SIZE - 2);
        voltageDrop.setCharacterSize(DEFAULT_FONT_SIZE - 2);
        ampsThrough.setCharacterSize(DEFAULT_FONT_SIZE - 2);

        title.setFillColor(GuiHelper::applyAlpha(sf::Color::White, alpha));
        voltageDrop.setFillColor(GuiHelper::applyAlpha(sf::Color::White, alpha));
        ampsThrough.setFillColor(GuiHelper::applyAlpha(sf::Color::White, alpha));

        title.setString(titleStr);
        voltageDrop.setString(voltageDropStr);
        ampsThrough.setString(ampsThroughStr);

        title.setPosition(float(GUI_X_PADDING + 15), float(GUI_Y_PADDING * 4 + 540));
        voltageDrop.setPosition(float(GUI_X_PADDING + 15), float(GUI_Y_PADDING * 4 + 590));
        ampsThrough.setPosition(float(GUI_X_PADDING + 15), float(GUI_Y_PADDING * 4 + 640));

        window.draw(title);
        window.draw(voltageDrop);
        window.draw(ampsThrough);

        if (selectedComponent->getType() != &ComponentTypes::WIRE) {
            std::string valueText = "Value:\n\t" + std::to_string(selectedComponent->getValue());

            if (selectedComponent->getType() == &ComponentTypes::RESISTOR)
                valueText += " Ohms";
            else
                valueText += " V";

            sf::Text value;
            value.setFont(DEFAULT_FONT);
            value.setCharacterSize(DEFAULT_FONT_SIZE - 2);
            value.setFillColor(GuiHelper::applyAlpha(sf::Color::White, alpha));
            value.setString(valueText);
            value.setPosition(float(GUI_X_PADDING + 15), float(GUI_Y_PADDING * 4 + 690));

            window.draw(value);
        }
    } else {
        sf::Text text;
        text.setFont(DEFAULT_FONT);
        text.setCharacterSize(DEFAULT_FONT_SIZE);
        text.setString("No Component Selected");
        text.setFillColor(GuiHelper::applyAlpha(sf::Color::White, alpha));
        text.setPosition({ float(GUI_X_PADDING + 30), float(GUI_Y_PADDING * 4 + 610) });

        window.draw(text);
    }
}

void ApplicationManager::drawComponent(Component* component) {
	sf::Vector2f posSpot = component->getPositive()->getVector();
	sf::Vector2f negSpot = component->getNegative()->getVector();

	sf::CircleShape c1(5.0f);
	c1.setPosition(posSpot - sf::Vector2f(5.0f, 5.0f));
    sf::Color color = GuiHelper::applyAlpha(COMPONENT_COLOR, alpha);
    c1.setFillColor(color);
	c1.setOutlineColor(sf::Color(120, 120, 120));
	c1.setOutlineThickness(1.0f);

	sf::CircleShape c2(c1);
	c2.setPosition(negSpot - sf::Vector2f(5.0f, 5.0f));

	window.draw(c1);
	window.draw(c2);

	switch (component->getType()->getValue()) {
		case 0:
			drawWire(component);
			break;
		case 1:
			drawResistor(component);
			break;
		case 2:
			drawVSrc(component);
			break;
		// TODO: Default
	}
}

// TODO: Generalize next three methods
void ApplicationManager::drawWire(Component* component) {
	sf::Vector2f posSpot = component->getPositive()->getVector();
	sf::Vector2f negSpot = component->getNegative()->getVector();

	double length = sqrt(pow(posSpot.x - negSpot.x, 2.0) + pow(posSpot.y - negSpot.y, 2.0));
	double degrees = 180 + (180 / 3.14159165) * atan2(posSpot.y - negSpot.y, posSpot.x - negSpot.x);

	sf::RectangleShape rect(sf::Vector2f(float(length), 2.0f));
    sf::Color color = GuiHelper::applyAlpha(COMPONENT_COLOR, alpha);
	rect.setFillColor(color);
	rect.setRotation(float(degrees));
	rect.setPosition(posSpot + sf::Vector2f(1.0f, -1.0f));

	window.draw(rect);
}

void ApplicationManager::drawResistor(Component* component) {
	sf::Vector2f posSpot = component->getPositive()->getVector();
	sf::Vector2f negSpot = component->getNegative()->getVector();

    double length = sqrt(pow(posSpot.x - negSpot.x, 2.0) + pow(posSpot.y - negSpot.y, 2.0));
	double radians = PI + atan2(posSpot.y - negSpot.y, posSpot.x - negSpot.x);

	sf::Texture tex = textures.at(0);
	sf::Vector2f spritePos(float(posSpot.x - (posSpot.x - negSpot.x) / 2.0 + sin(radians) * tex.getSize().x / 4.0),
						   float(posSpot.y - (posSpot.y - negSpot.y) / 2.0 - cos(radians) * tex.getSize().y / 2.0));

	sf::RectangleShape rect1(sf::Vector2f(float(0.5 * (length - tex.getSize().x)), 2.0f));
    sf::Color color = GuiHelper::applyAlpha(COMPONENT_COLOR, alpha);
	rect1.setFillColor(color);
	rect1.setRotation(float(radians * 180.0 / PI));
	rect1.setPosition(posSpot + sf::Vector2f(1.0f, -1.0f));

	sf::RectangleShape rect2(rect1);
	rect2.setRotation(float((radians + PI) * 180.0 / PI));
	rect2.setPosition(negSpot + sf::Vector2f(-1.0f, 1.0f));

	sf::Sprite resistor;
	resistor.setOrigin(sf::Vector2f(float(tex.getSize().x / 2.0), 0.0));
	resistor.setTexture(tex);
	resistor.setPosition(spritePos);
	resistor.setRotation(float(radians * (180.0 / PI)));
	resistor.setColor(color);

	window.draw(rect1);
	window.draw(rect2);
	window.draw(resistor);
}

void ApplicationManager::drawVSrc(Component* component) {
	sf::Vector2f posSpot = component->getPositive()->getVector();
	sf::Vector2f negSpot = component->getNegative()->getVector();

    double length = sqrt(pow(posSpot.x - negSpot.x, 2.0) + pow(posSpot.y - negSpot.y, 2.0));
    double radians = PI + atan2(posSpot.y - negSpot.y, posSpot.x - negSpot.x);

	sf::Texture tex = textures.at(1);
	sf::Vector2f spritePos(float(posSpot.x - (posSpot.x - negSpot.x) / 2.0 + sin(radians) * tex.getSize().x / 2.0),
		                   float(posSpot.y - (posSpot.y - negSpot.y) / 2.0 - cos(radians) * tex.getSize().y / 2.0));

	sf::RectangleShape rect1(sf::Vector2f(float(0.5 * (length - tex.getSize().x)), 2.0f));
    sf::Color color = GuiHelper::applyAlpha(COMPONENT_COLOR, alpha);
	rect1.setFillColor(color);
	rect1.setRotation(float(radians * 180.0 / PI));
	rect1.setPosition(posSpot + sf::Vector2f(1.0f, -1.0f));

	sf::RectangleShape rect2(rect1);
	rect2.setRotation(float((radians + PI) * 180.0 / PI));
	rect2.setPosition(negSpot + sf::Vector2f(-1.0f, 1.0f));

	sf::Sprite vsrc;
	vsrc.setOrigin(sf::Vector2f(float(tex.getSize().x / 2.0), 0.0));
	vsrc.setTexture(tex);
	vsrc.setPosition(spritePos);
	vsrc.setRotation(float(radians * (180.0 / PI)));
	vsrc.setColor(color);

	window.draw(rect1);
	window.draw(rect2);
	window.draw(vsrc);
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

        selectedComponent->setValue(val);
    }
}