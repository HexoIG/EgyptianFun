#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "TextDrawer.h"

class Dialog
{
private:
	const int width = 16 * 40;
	const int height = 16 * 6;
	int speaker_image_scale = 4;
	const sf::Color fill_color = sf::Color::White;

	sf::SoundBuffer sound_buffer;
	sf::Sound sound;
	sf::SoundBuffer click_buffer;
	sf::Sound click_sound;

	std::string speaker;
	std::string dialog;
	TextDrawer text_drawer;
	int dialog_index = 0;
	int starting_frame = 0;
	int x = 0;
	int y = 0;

	sf::RectangleShape rect;
	sf::Texture texture;
	sf::Sprite sprite;

	void drawCurrentDialog(sf::RenderWindow* window);

public:
	int speaker_image_side_length = 19 * 4;
	bool show_next = false;
	bool skipped = false;
	bool active = false;
	int language_type = 0;
	int typing_interval = 1;

	Dialog(int x, int y);
	void updateDialog();
	void setNewDialog(std::string speaker, std::string dialog, int starting_frame);
	void drawDialog(sf::RenderWindow* window, int frame);
};

