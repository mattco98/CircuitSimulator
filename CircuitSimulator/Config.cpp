/**
    Author:             Matthew Olsson
    File Title:         Config.h
    File Description:   Implements all constant configuration variables. 
                        Enables multiple files to use the same constants 
                        without redeclaration.
    Due Date:           4/25/2018
    Date Created:       3/26/2018
    Date Last Modified: 4/23/2018
*/

#include "Config.h"

// General
const float PI = 3.141592f;

// Get default font
sf::Font getDefaultFont() {
	sf::Font font;
	font.loadFromFile("assets/Menlo.ttf");
	return font;
};

// APPLICATION
const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;

// GUI
const int GUI_X_PADDING = 30;
const int GUI_Y_PADDING = 30;
const sf::Color BACKGROUND_COLOR = { 40, 40, 45 };
const sf::Color BORDER_COLOR = { 200, 200, 200 };

// Panel
const sf::Vector2f PANEL_TITLE_1    = { float(GUI_X_PADDING), float(GUI_Y_PADDING) };
const sf::Vector2f PANEL_TITLE_2    = { float(GRID_LEFT_OFFSET - GUI_X_PADDING), float(GUI_Y_PADDING) };
const sf::Vector2f PANEL_TITLE_3    = { float(GRID_LEFT_OFFSET - GUI_X_PADDING), float(GUI_Y_PADDING + 60) };
const sf::Vector2f PANEL_TITLE_4    = { float(GUI_X_PADDING), float(GUI_Y_PADDING + 60) };
const sf::Vector2f PANEL_COMP_1     = { float(GUI_X_PADDING), float(GUI_Y_PADDING * 2 + 60) };
const sf::Vector2f PANEL_COMP_2     = { float(GRID_LEFT_OFFSET - GUI_X_PADDING), float(GUI_Y_PADDING * 2 + 60) };
const sf::Vector2f PANEL_COMP_3     = { float(GRID_LEFT_OFFSET - GUI_X_PADDING), float(GUI_Y_PADDING * 2 + 200) };
const sf::Vector2f PANEL_COMP_4     = { float(GUI_X_PADDING), float(GUI_Y_PADDING * 2 + 200) };
const sf::Vector2f PANEL_INSTRUCT_1 = { float(GUI_X_PADDING), float(GUI_Y_PADDING * 3 + 200) };
const sf::Vector2f PANEL_INSTRUCT_2 = { float(GRID_LEFT_OFFSET - GUI_X_PADDING), float(GUI_Y_PADDING * 3 + 200) };
const sf::Vector2f PANEL_INSTRUCT_3 = { float(GRID_LEFT_OFFSET - GUI_X_PADDING), float(GUI_Y_PADDING * 3 + 530) };
const sf::Vector2f PANEL_INSTRUCT_4 = { float(GUI_X_PADDING), float(GUI_Y_PADDING * 3 + 530) };
const sf::Vector2f PANEL_INFO_1     = { float(GUI_X_PADDING), float(GUI_Y_PADDING * 4 + 530) };
const sf::Vector2f PANEL_INFO_2     = { float(GRID_LEFT_OFFSET - GUI_X_PADDING), float(GUI_Y_PADDING * 4 + 530) };
const sf::Vector2f PANEL_INFO_3     = { float(GRID_LEFT_OFFSET - GUI_X_PADDING), float(SCREEN_HEIGHT - GUI_Y_PADDING) };
const sf::Vector2f PANEL_INFO_4     = { float(GUI_X_PADDING), float(SCREEN_HEIGHT - GUI_Y_PADDING) };

// GRID
const int SPOT_SPACING = 40;
const int GRID_TOP_OFFSET = GUI_Y_PADDING;
const int GRID_BOTTOM_OFFSET = GUI_Y_PADDING;
const int GRID_LEFT_OFFSET = 350;
const int GRID_RIGHT_OFFSET = GUI_X_PADDING;

// MENU
const sf::Color DEFAULT_COLOR = { 225, 225, 225 };
const sf::Color WARNING_COLOR = { 239, 124, 124 };
const sf::Font DEFAULT_FONT = getDefaultFont();
const unsigned int DEFAULT_FONT_SIZE = 18;

// Components
const sf::Color COMPONENT_COLOR = { 170, 170, 170 };
const sf::Color COMPONENT_HOVER_COLOR = { 255, 255, 255 };
const sf::Color COMPONENT_SELECTED_COLOR = { 64, 224, 208 };