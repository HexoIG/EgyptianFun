#include "BackgroundDrawer.h"

BackgroundDrawer::BackgroundDrawer(int state)
{
	setState(state);
	background.setScale(8, 8);
}

void BackgroundDrawer::updateBackground()
{
	texture.loadFromFile("ResourceFiles/Backgrounds/Background" + std::to_string(state) + ".png");
	background.setTexture(texture);
}

int BackgroundDrawer::getState()
{
	return state;
}

void BackgroundDrawer::setState(int new_state)
{
	state = new_state;
	updateBackground();
}

void BackgroundDrawer::drawBackground(sf::RenderWindow* window)
{
	window->draw(background);
}