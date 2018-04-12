#include "Config.h"

// Get default font
sf::Font getDefaultFont() {
	sf::Font font;
	font.loadFromFile("Menlo.ttf");
	return font;
};

// APPLICATION
const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;

// GUI
const int GUIXPADDING = 30;
const int GUIYPADDING = 30;

// GRID
const int SPOT_SPACING = 40;
const int GRID_X_OFFSET = 350;
const int GRID_Y_OFFSET = 30;

// MENU
const sf::Color DEFAULT_COLOR = sf::Color(225, 225, 225);
const sf::Font DEFAULT_FONT = getDefaultFont();
const unsigned int DEFAULT_FONT_SIZE = 18;

// Components
const sf::Color COMPONENT_COLOR(64, 224, 208);