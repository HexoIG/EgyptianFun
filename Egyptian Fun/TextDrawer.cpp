#include "TextDrawer.h"

TextDrawer::TextDrawer()
{
	texture.loadFromFile("ResourceFiles/Letters.png");
	sprite.setTexture(texture);
	sprite.setScale(text_scale, text_scale);
}

void TextDrawer::drawText(sf::RenderWindow* window, std::string string, int language_type)
{
	for (char& c : string) 
	{
		c = std::tolower(c);
	}

	int current_x = 0;
	int line = 0;
	
	for (int i = 0; i < string.length(); i++)
	{
		if (string[i] == '\n') 
		{ 
			line++; 
			current_x = 0;
			continue; 
		}

		int letter_index = letter_indices.at(string[i]);
		int letter_width = letter_widths.at(string[i]);

		sprite.setTextureRect(sf::IntRect(letter_index * 5, language_type * 6, 5, 5));
		sprite.setPosition(sf::Vector2f(x + current_x * text_scale, y + line * line_spacing * text_scale));

		window->draw(sprite);

		current_x += letter_width + 1;
	}
}