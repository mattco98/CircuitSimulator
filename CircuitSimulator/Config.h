/**
    Author:             Matthew Olsson
    File Title:         Config.h
    File Description:   Declares all constant configuration variables. Enables
                        multiple files to use the same constants without
                        redeclaration.
    Due Date:           4/25/2018
    Date Created:       3/26/2018
    Date Last Modified: 4/23/2018
*/

#pragma once

#include <string>              // string class
#include <SFML/Graphics.hpp>   // sf::Color, sf::Vector2f

// General
extern const float PI;

extern void set(int width, int height);

// Application
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

// GUI
extern const int GUI_X_PADDING;
extern const int GUI_Y_PADDING;
extern const sf::Color BACKGROUND_COLOR;
extern const sf::Color BORDER_COLOR;

// Panel border bounds
extern sf::Vector2f PANEL_TITLE_1;
extern sf::Vector2f PANEL_TITLE_2;
extern sf::Vector2f PANEL_TITLE_3;
extern sf::Vector2f PANEL_TITLE_4;
extern sf::Vector2f PANEL_COMP_1;
extern sf::Vector2f PANEL_COMP_2;
extern sf::Vector2f PANEL_COMP_3;
extern sf::Vector2f PANEL_COMP_4;
extern sf::Vector2f PANEL_INSTRUCT_1;
extern sf::Vector2f PANEL_INSTRUCT_2;
extern sf::Vector2f PANEL_INSTRUCT_3;
extern sf::Vector2f PANEL_INSTRUCT_4;
extern sf::Vector2f PANEL_INFO_1;
extern sf::Vector2f PANEL_INFO_2;
extern sf::Vector2f PANEL_INFO_3;
extern sf::Vector2f PANEL_INFO_4;
	
// Grid
extern const int SPOT_SPACING;
extern const int GRID_TOP_OFFSET;
extern const int GRID_BOTTOM_OFFSET;
extern const int GRID_LEFT_OFFSET;
extern const int GRID_RIGHT_OFFSET;

// Menu
extern const sf::Color DEFAULT_COLOR;
extern const sf::Color WARNING_COLOR;
extern const sf::Font DEFAULT_FONT;
extern const unsigned int DEFAULT_FONT_SIZE;

// Components
extern const sf::Color COMPONENT_COLOR;
extern const sf::Color COMPONENT_HOVER_COLOR;
extern const sf::Color COMPONENT_SELECTED_COLOR;