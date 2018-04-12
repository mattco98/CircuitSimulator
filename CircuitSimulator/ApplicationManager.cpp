#include <iostream>
#include "ApplicationManager.h"
#include "ComponentTypes.h"

// TODO: Debug
using std::cout;
using std::endl;

// TODO: const double?
#define PI 3.14159265

ApplicationManager::ApplicationManager(sf::VideoMode mode, std::string windowTitle, sf::Uint32 style) :
	// Initialize window and grid
	window(mode, windowTitle, style),
	grid(mode.width, mode.height, GRID_X_OFFSET, GRID_Y_OFFSET),
	selectedComponentType(ComponentTypes::WIRE) {

	// Initialize textures
	sf::Texture resistor,
				vSrc;
	if (!resistor.loadFromFile("resistor.png") || !vSrc.loadFromFile("vsrc.png")) {
		// TODO
	}
	textures.push_back(resistor);
	textures.push_back(vSrc);
};

ApplicationManager::~ApplicationManager() {
	delete selectedComponent;
}

void ApplicationManager::update() {
	window.clear(sf::Color(32, 34, 37));

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
		case sf::Event::MouseButtonPressed:
			handleMousePressed(event.mouseButton);
			break;
		case sf::Event::Closed:
			handleClosed();
			break;
	}
}

void ApplicationManager::handleKeypress(sf::Event::KeyEvent event) {
	unsigned short key = event.code;
	cout << key << endl;
	unsigned int i;

	switch (key) {
		case sf::Keyboard::Escape:
			handleClosed();
			break;
		case sf::Keyboard::Up:
			i = getSelectedComponentType().getValue();

			if (i == 0)
				i = ComponentTypes::NUM_TYPES - 1;
			else
				i--;

			setSelectedComponentType(ComponentTypes::getType(i));
			break;
		case sf::Keyboard::Down:
			i = getSelectedComponentType().getValue() + 1;

			if (i > ComponentTypes::NUM_TYPES - 1)
				i = 0;

			setSelectedComponentType(ComponentTypes::getType(i));
			break;
		case sf::Keyboard::C:
			grid.clearComponents();
			break;
		// TODO: case default
	}
}

void ApplicationManager::handleMousePressed(sf::Event::MouseButtonEvent event) {
	if (event.button == sf::Mouse::Left) {
		Spot* spot;
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		grid.getNearestSpot(mousePos, &spot);

		if (!isPlacing) {
			selectedComponent = new Component(selectedComponentType);
			selectedComponent->setPositive(&spot);
			isPlacing = true;
		} else {
			// TODO: Validity check for selectedComponent
			selectedComponent->setNegative(&spot);

			grid.addComponent(selectedComponent);
			spot->addComponent(selectedComponent);

			isPlacing = false;
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

ComponentType ApplicationManager::getSelectedComponentType() {
	return selectedComponentType;
}

void ApplicationManager::setSelectedComponentType(ComponentType type) {
	selectedComponentType = type;
}


///////////////////////
// Rendering methods //
///////////////////////

// Main rendering methods
void ApplicationManager::draw() {
	drawGui();
	for (auto component : grid.getComponents()) {
		drawComponent(component);
	}
    if (isPlacing) {
        Component* hover = selectedComponent;
        Spot* spot;
        grid.getNearestSpot(sf::Mouse::getPosition(window), &spot);
        hover->setNegative(&spot);
        drawComponent(hover);
    }
}

void ApplicationManager::drawGui() {
	bool debug = false;

	// Draw debug objects
	if (debug) {
		// Draw grid spots
		for (auto spot : grid.getSpots()) {
			sf::CircleShape spotCircle(3.0f);
			spotCircle.setFillColor(sf::Color::Red);
			spotCircle.setPosition(float(spot->x - 3), float(spot->y - 3));
			window.draw(spotCircle);
		}
	}

	// Draw sidebar rectangle
	drawRectangleHollow(sf::Vector2f((float) GUIXPADDING, (float) GUIYPADDING),
		sf::Vector2f(float(GRID_X_OFFSET - GUIXPADDING), (float) GUIYPADDING),
		sf::Vector2f(float(GRID_X_OFFSET - GUIXPADDING), float(SCREEN_HEIGHT - GUIYPADDING)),
		sf::Vector2f((float) GUIXPADDING, float(SCREEN_HEIGHT - GUIYPADDING)),
		sf::Color(200, 200, 200));

	// Draw component menu
	drawComponentMenu();

	// Draw dot on nearest grid spot
	Spot* nearestSpot;

	sf::Vector2i mousePos(sf::Mouse::getPosition(window));
	if (!grid.getNearestSpot(mousePos, &nearestSpot)) {
		return;
	}

	sf::CircleShape mouseCircle(7.0f);
	mouseCircle.setFillColor(sf::Color(74, 77, 82));
	mouseCircle.setOutlineColor(sf::Color(47, 49, 54));
	mouseCircle.setOutlineThickness(1.0f);
	mouseCircle.setPosition(float(nearestSpot->x - 7), float(nearestSpot->y - 7));

	window.draw(mouseCircle);
}

void ApplicationManager::drawComponentMenu() {
	int xPos = GUIXPADDING + 40,
		yPos = GUIYPADDING + 100,
		index = 0;

	sf::Text title;
	title.setFont(DEFAULT_FONT);
	title.setCharacterSize(DEFAULT_FONT_SIZE + 6);
	title.setString("Components:");
	title.setPosition(float(xPos), float(yPos));

	window.draw(title);

	yPos += 40;

	for (int i = 0; i < ComponentTypes::NUM_TYPES; i++) {
		std::string name = ComponentTypes::getType(i).getName();

		sf::Text text;
		text.setFont(DEFAULT_FONT);
		text.setCharacterSize(DEFAULT_FONT_SIZE);
		text.setString(name);
		text.setPosition(float(xPos), float(yPos));

		if (i == getSelectedComponentType().getValue()) {
			text.setFillColor(sf::Color(40, 40, 40));
			sf::RectangleShape rect(sf::Vector2f(200, 20));
			rect.setFillColor(sf::Color(225, 225, 225));
			rect.setPosition(sf::Vector2f(float(xPos - 3), float(yPos + 1)));
			window.draw(rect);
		}

		window.draw(text);

		yPos += 20;
		index++;
	}
}

void ApplicationManager::drawComponent(Component* component) {
	sf::Vector2f posSpot = component->getPositive()->getVector();
	sf::Vector2f negSpot = component->getNegative()->getVector();

	sf::CircleShape c1(5.0f);
	c1.setPosition(posSpot - sf::Vector2f(5.0f, 5.0f));
	c1.setFillColor(COMPONENT_COLOR);
	c1.setOutlineColor(sf::Color(120, 120, 120));
	c1.setOutlineThickness(1.0f);

	sf::CircleShape c2(c1);
	c2.setPosition(negSpot - sf::Vector2f(5.0f, 5.0f));

	window.draw(c1);
	window.draw(c2);

	switch (component->getType().getValue()) {
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

	sf::RectangleShape rect(sf::Vector2f(length, 2.0f));
	rect.setFillColor(COMPONENT_COLOR);
	rect.setRotation(degrees);
	rect.setPosition(posSpot + sf::Vector2f(1.0f, -1.0f));

	window.draw(rect);
}

void ApplicationManager::drawResistor(Component* component) {
	sf::Vector2f posSpot = component->getPositive()->getVector();
	sf::Vector2f negSpot = component->getNegative()->getVector();

	double length = sqrt(pow(posSpot.x - negSpot.x, 2.0) + pow(posSpot.y - negSpot.y, 2.0));
	double radians = PI + atan2(posSpot.y - negSpot.y, posSpot.x - negSpot.x);

	sf::Texture tex = textures.at(0);
	sf::Vector2f spritePos(posSpot.x - (posSpot.x - negSpot.x) / 2 + sin(radians) * tex.getSize().x / 4,
						   posSpot.y - (posSpot.y - negSpot.y) / 2 - cos(radians) * tex.getSize().y / 2);

	sf::RectangleShape rect1(sf::Vector2f(0.5 * (length - tex.getSize().x), 2.0f));
	rect1.setFillColor(COMPONENT_COLOR);
	rect1.setRotation(radians * 180 / PI);
	rect1.setPosition(posSpot + sf::Vector2f(1.0f, -1.0f));

	sf::RectangleShape rect2(rect1);
	rect2.setRotation((radians + PI) * 180 / PI);
	rect2.setPosition(negSpot + sf::Vector2f(-1.0f, 1.0f));

	sf::Sprite resistor;
	resistor.setOrigin(sf::Vector2f(tex.getSize().x / 2, 0));
	resistor.setTexture(tex);
	resistor.setPosition(spritePos);
	resistor.setRotation(radians * (180 / PI));
	resistor.setColor(COMPONENT_COLOR);

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
	sf::Vector2f spritePos(posSpot.x - (posSpot.x - negSpot.x) / 2 + sin(radians) * tex.getSize().x / 2,
		posSpot.y - (posSpot.y - negSpot.y) / 2 - cos(radians) * tex.getSize().y / 2);

	sf::RectangleShape rect1(sf::Vector2f(0.5 * (length - tex.getSize().x), 2.0f));
	rect1.setFillColor(COMPONENT_COLOR);
	rect1.setRotation(radians * 180 / PI);
	rect1.setPosition(posSpot + sf::Vector2f(1.0f, -1.0f));

	sf::RectangleShape rect2(rect1);
	rect2.setRotation((radians + PI) * 180 / PI);
	rect2.setPosition(negSpot + sf::Vector2f(-1.0f, 1.0f));

	sf::Sprite vsrc;
	vsrc.setOrigin(sf::Vector2f(tex.getSize().x / 2, 0));
	vsrc.setTexture(tex);
	vsrc.setPosition(spritePos);
	vsrc.setRotation(radians * (180 / PI));
	vsrc.setColor(COMPONENT_COLOR);

	window.draw(rect1);
	window.draw(rect2);
	window.draw(vsrc);
}


////////////////////
// Helper Methods //
////////////////////

void ApplicationManager::drawLine(sf::Vertex p1, sf::Vertex p2) {
	const sf::Vertex vertices[2] = { p1, p2 };
	window.draw(vertices, 2, sf::PrimitiveType::Lines);
}

void ApplicationManager::drawLine(sf::Vector2f p1, sf::Vector2f p2, sf::Color color) {
	sf::Vertex v1(p1),
			   v2(p2);

	v1.color = color;
	v2.color = color;

	drawLine(v1, v2);
}

void ApplicationManager::drawRectangleHollow(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4, sf::Color color) {
	drawLine(p1, p2, color);
	drawLine(p2, p3, color);
	drawLine(p3, p4, color);
	drawLine(p4, p1, color);
}