#include "Dialog.h"

bool checkIfInRectDialog(sf::Vector2i vec2, int x, int y, int left, int bottom)
{
	return (vec2.x > x && vec2.x < left&& vec2.y > y && vec2.y < bottom);
}

Dialog::Dialog(int x, int y)
{
	this->x = x;
	this->y = y;
	speaker = "EMPTY";
	dialog = "";

	rect.setFillColor(fill_color);
	rect.setOutlineColor(sf::Color::Black);
	rect.setOutlineThickness(4);
	rect.setSize(sf::Vector2f(width, height));

	updateDialog();
}

void Dialog::updateDialog()
{
	texture.loadFromFile("ResourceFiles/SpeakerImages/" + speaker + ".png");
	sprite = sf::Sprite();
	sprite.setScale(speaker_image_scale, speaker_image_scale);
	sprite.setTexture(texture);
	sprite.setPosition(
		x + (height - speaker_image_side_length) / 2,
		y + (height - speaker_image_side_length) / 2
	);
	rect.setPosition(x, y);
	skipped = false;
}

void Dialog::setNewDialog(std::string speaker, std::string dialog, int starting_frame)
{
	this->speaker = speaker;
	this->dialog = dialog;
	this->starting_frame = starting_frame;
	dialog_index = 0;
	
	updateDialog();
}

void Dialog::drawCurrentDialog(sf::RenderWindow* window)
{
	text_drawer.x = x + height;
	text_drawer.y = y + height - speaker_image_side_length - 4;
	text_drawer.drawText(window, dialog.substr(0, fmin(dialog_index, dialog.length())), language_type);
}

void Dialog::drawDialog(sf::RenderWindow* window, int frame)
{
	skipped = false;
	if (!active) { return; }

	window->draw(rect);
	window->draw(sprite);

	if (frame - starting_frame < 0) { return; }

	dialog_index = (frame - starting_frame) / typing_interval;
	drawCurrentDialog(window);

	if (show_next && dialog_index >= dialog.length() + 4)
	{
		text_drawer.x = text_drawer.x = x + width - 20 * text_drawer.text_scale;
		text_drawer.y = y + height - 7 * text_drawer.text_scale;
		text_drawer.drawText(window, "Next", 0);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && checkIfInRectDialog(sf::Mouse::getPosition(*window), x, y, x + width, y + height))
		{
			skipped = true;
		}
	}
}