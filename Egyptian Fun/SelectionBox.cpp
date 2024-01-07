#include "SelectionBox.h"

bool checkIfInRectSelection(sf::Vector2i vec2, int x, int y, int left, int bottom)
{
	return (vec2.x > x && vec2.x < left && vec2.y > y && vec2.y < bottom);
}

SelectionBox::SelectionBox(int x, int bottom_y)
{
	this->x = x;
	this->bottom_y = bottom_y;

	rect.setFillColor(fill_color);
	rect.setOutlineColor(sf::Color::Black);
	rect.setOutlineThickness(4);

	response_rect.setFillColor(fill_color);
	response_rect.setOutlineColor(sf::Color::Black);
	response_rect.setOutlineThickness(4);

	outline_rect.setFillColor(sf::Color::Transparent);
	outline_rect.setOutlineColor(sf::Color::Black);
	outline_rect.setOutlineThickness(4);

	updateSelectionBox();
}

void SelectionBox::setNewSelections(std::string text, int selections, std::vector<int> selection_lines, int starting_frame)
{
	this->text = text;
	this->selections = selections;
	this->selection_lines = selection_lines;
	this->starting_frame = starting_frame;
	text_index = 0;
	
	updateSelectionBox();
}

void SelectionBox::updateSelectionBox()
{
	int lines = 0;

	for (int i = 0; i < selections; i++)
	{
		lines += selection_lines[i];
	}
	line_count = lines;

	rect.setPosition(x, bottom_y - lines * text_drawer.text_scale * 6 - padding * 2);
	rect.setSize(sf::Vector2f(width, lines * (text_drawer.text_scale * 6 - 1) + padding * 2));

	response_rect.setPosition(
		x + width / 2 - 19 * text_drawer.text_scale + 2, 
		bottom_y - line_count * text_drawer.text_scale * 6 - padding - 15 * text_drawer.text_scale
	);
	response_rect.setSize(sf::Vector2f(20 * text_drawer.text_scale * 2, 20 * text_drawer.text_scale));
}

void SelectionBox::drawCurrentText(sf::RenderWindow* window, int frame)
{
	text_drawer.x = x + padding;
	text_drawer.y = bottom_y - line_count * text_drawer.text_scale * 6 - padding;
	text_drawer.drawText(window, text.substr(0, fmin(text_index, text.length())), 0);
}

void SelectionBox::drawSelectionBox(sf::RenderWindow* window, int frame)
{
	if (!active) { clicking = false; return; }

	window->draw(response_rect);
	window->draw(rect);
	text_drawer.x = x + width / 2 - 15 * text_drawer.text_scale;
	text_drawer.y = bottom_y - line_count * text_drawer.text_scale * 6 - padding - 13 * text_drawer.text_scale;
	text_drawer.drawText(window, "RESPONSE", 0);

	if (frame - starting_frame < 0) { return; }

	text_index = (frame - starting_frame) / typing_interval;
	drawCurrentText(window, frame);

	if (selected != -1)
	{
		window->draw(outline_rect);
	}
}

void SelectionBox::updateSelection(sf::RenderWindow* window)
{
	clicking = false;
	if (!active) { return; }

	sf::Vector2i mouse_position = sf::Mouse::getPosition(*window);

	if (!checkIfInRectSelection(mouse_position, x + padding, bottom_y - rect.getLocalBounds().height + padding, x + width, bottom_y - padding - 1)) { selected = -1; return; }

	int current_y = 0;
	for (int i = 0; i < selection_lines.size(); i++)
	{
		if (mouse_position.y < current_y * text_drawer.text_scale * 6 + (text_drawer.text_scale * 6 * selection_lines[i]) + bottom_y - rect.getLocalBounds().height + padding)
		{
			selected = i;
			break;
		}
		current_y += selection_lines[i];

		if (current_y == line_count) { selected = -1; return; }
	}
	
	outline_rect.setPosition(
		x + padding - 4, 
		current_y * text_drawer.text_scale * 6 + 3 + bottom_y - (line_count + 2) * (text_drawer.text_scale * 6) + padding
	);
	outline_rect.setSize(sf::Vector2f(
		width - padding * 2, 
		text_drawer.text_scale * 6 * selection_lines[selected] - 1));

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		clicking = true;
	}
}