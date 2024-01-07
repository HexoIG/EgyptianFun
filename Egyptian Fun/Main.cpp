#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <Windows.h>

#include "PrecompiledHeaders.h"


const int WINDOW_WIDTH = 128 * 8;
const int WINDOW_HEIGHT = 96 * 8;
const int FPS = 50;

int main()
{
	//------// SFML SETUP //------//
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Egyptian Fun", sf::Style::Close);

	
	//------// MAIN LOOP SETUP //------//
	GameLogic game_logic;

	int frame = 0;
	double fixed_frame = frame;
	auto last_time = std::chrono::steady_clock::now();
	auto current_time = last_time;
	double delta_time = 0;
	int last_fixed_frame = -1;


	//------// MAIN LOOP //------//
	while (window.isOpen())
	{
		// Delta Time //
		current_time = std::chrono::steady_clock::now();
		delta_time = (std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - last_time).count())  /  (1.0 / FPS * 1000000000);
		last_time = current_time;
		
		// SFML Events //
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Render Screen //
		if ((int)fixed_frame != last_fixed_frame)
		{
			last_fixed_frame = (int)fixed_frame;

			window.clear();

			game_logic.updateGame(&window, &fixed_frame);

			window.display();
		}
		

		// End Frame //
		frame++;
		fixed_frame += delta_time;
	}


	//------// APPLICATION CLOSING //------//
	return 0;
}