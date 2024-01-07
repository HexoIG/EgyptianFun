#pragma once

#include <SFML/Graphics.hpp>

class BackgroundDrawer
{
private:
	int state;
	sf::Texture texture;
	sf::Sprite background;

	void updateBackground();

public:
	BackgroundDrawer(int state);
	int getState();
	void setState(int new_state);
	void drawBackground(sf::RenderWindow* window);
};

