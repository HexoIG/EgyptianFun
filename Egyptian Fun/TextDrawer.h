#pragma once

#include <SFML/Graphics.hpp>

class TextDrawer
{
private:
	const std::map<char, int> letter_indices = {
		{'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}, {'e', 4}, {'f', 5}, {'g', 6}, {'h', 7}, {'i', 8}, {'j', 9}, 
		{'k', 10},{'l', 11},{'m', 12},{'n', 13},{'o', 14},{'p', 15},{'q', 16},{'r', 17},{'s', 18},{'t', 19}, 
		{'u', 20},{'v', 21},{'w', 22},{'x', 23},{'y', 24},{'z', 25}, {' ', 26}, {')', 27}, {'.', 28}, {'?', 29}
	};

	const std::map<char, int> letter_widths = {
		{'a', 3}, {'b', 3}, {'c', 3}, {'d', 3}, {'e', 3}, {'f', 3}, {'g', 4}, {'h', 3}, {'i', 3}, {'j', 3},
		{'k', 3},{'l', 3},{'m', 5},{'n', 5},{'o', 3},{'p', 3},{'q', 4},{'r', 3},{'s', 3},{'t', 3},
		{'u', 3},{'v', 3},{'w', 5},{'x', 3},{'y', 3},{'z', 3}, {' ', 2}, {')', 2}, {'.', 1}, {'?', 2}
	};

	sf::Texture texture;
	sf::Sprite sprite;

public:
	const int text_scale = 3;
	const int line_spacing = 6;
	int x = 0;
	int y = 0;

	TextDrawer();
	void drawText(sf::RenderWindow* window, std::string string, int language_type);
};

