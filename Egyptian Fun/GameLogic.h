#pragma once

#include "PrecompiledHeaders.h"

#include <SFML/Graphics.hpp>

class GameLogic
{
public:
	BackgroundDrawer background_drawer = BackgroundDrawer(0);
	TextDrawer text_drawer = TextDrawer();
	Dialog dialog = Dialog(16 * 12, 82 * 8);
	SelectionBox selection_box = SelectionBox(16 * 12, 80 * 8);
	sf::RectangleShape ending_rect = sf::RectangleShape(sf::Vector2f(32, 32));

	void updateGame(sf::RenderWindow* window, double* frame_p);
	void draw(sf::RenderWindow* window, int frame);
};

