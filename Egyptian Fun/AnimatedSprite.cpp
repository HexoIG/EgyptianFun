#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(std::string file_name, int width, int height, int animation_frames)
{
	texture.loadFromFile("ResourceFiles/Sprites/" + file_name + ".png");
	sprite.setTexture(texture);
	this->width = width;
	this->height = height;
	this->animation_frames = animation_frames;
}

void AnimatedSprite::draw(sf::RenderWindow* window, int frame)
{
	sprite.setScale(scale, scale);
	sprite.setTextureRect(sf::IntRect(((frame / animation_interval) % animation_frames) * (width + 1), 0, width, height));
	sprite.setPosition(x, y);
	window->draw(sprite);
}