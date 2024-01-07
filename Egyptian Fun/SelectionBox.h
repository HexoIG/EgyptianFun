#pragma once

#include "TextDrawer.h"

class SelectionBox
{
private:
	const int width = 16 * 40;
	const int padding = 16;
	const sf::Color fill_color = sf::Color::White;

	sf::RectangleShape rect;
	sf::RectangleShape response_rect;
	sf::RectangleShape outline_rect;
	TextDrawer text_drawer;

	std::vector<int> selection_lines;
	std::string text = "";
	int line_count = 0;
	int selections = 0;
	int starting_frame = 0;
	int text_index = 0;

	int x = 0;
	int bottom_y = 0;

	void drawCurrentText(sf::RenderWindow* window, int frame);

public:
	int selected = -1;
	bool clicking = false;
	bool active = false;
	int typing_interval = 1;

	SelectionBox(int x, int bottom_y);
	void setNewSelections(std::string text, int selections, std::vector<int> selection_lines, int starting_frame);
	void updateSelectionBox();
	void drawSelectionBox(sf::RenderWindow* window, int frame);
	void updateSelection(sf::RenderWindow* window);
};

