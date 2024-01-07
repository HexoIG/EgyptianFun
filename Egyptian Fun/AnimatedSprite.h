#pragma once

#include <SFML/Graphics.hpp>

class AnimatedSprite
{
private:
	sf::Texture texture;
	sf::Sprite sprite;

	int animation_frames;
	int width;
	int height;

public:
	int scale = 1;
	int animation_interval = 1;
	int x = 0;
	int y = 0;

	AnimatedSprite(std::string file_name, int width, int height, int animation_frames);
	void draw(sf::RenderWindow* window, int frame);
};

