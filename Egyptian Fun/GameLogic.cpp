#include "GameLogic.h"

#include <SFML/Audio.hpp>

bool checkIfInRectLogic(sf::Vector2i vec2, int x, int y, int left, int bottom)
{
	return (vec2.x > x && vec2.x < left && vec2.y > y && vec2.y < bottom);
}

//---------// HARD CODED GAME LOGIC //---------//

void GameLogic::updateGame(sf::RenderWindow* window, double* frame_p)
{
	int frame = (int)*frame_p;

	static bool hamburger_ending = false;
	static bool toenail_ending = false;
	static bool true_ending = false;

	static bool game_started = false;
	static int game_started_frame = -1;
	static int ending_frame = -1;
	static int calls = 0;
	static int last_frame = 0;
	static int conversation_state = 0;
	static int answer_state = 0;
	static int response_frame = -1;
	static int response = -1;
	static int next_answer_state = -1;
	static int skip_num = 0;
	static int talking_frame = -1;
	static AnimatedSprite main_character_still = AnimatedSprite("MainCharacterStill", 13, 25, 1);
	static AnimatedSprite main_character_walk = AnimatedSprite("MainCharacterWalk", 13, 25, 4);
	static AnimatedSprite ufo_spin = AnimatedSprite("UFOSpin", 16, 16, 3);
	static sf::RectangleShape start_rect = sf::RectangleShape(sf::Vector2f(300, 100));
	static TextDrawer start_text_drawer = TextDrawer();
	std::vector<int> ints;

	static AnimatedSprite true_t = AnimatedSprite("True", 8, 8, 1);
	static AnimatedSprite hamburger_t = AnimatedSprite("Burger", 8, 8, 1);
	static AnimatedSprite toenail_t = AnimatedSprite("Toenail", 8, 8, 1);

	static sf::SoundBuffer opening_song_buffer;
	static sf::SoundBuffer explosion_buffer;
	static sf::SoundBuffer ending_buffer;

	static sf::Sound opening_song;
	static sf::Sound explosion_sound;
	static sf::Sound ending_sound;


	calls++;

	if (calls == 1)
	{
		opening_song_buffer.loadFromFile("ResourceFiles/Audio/EgyptianFunOpeningSong.wav");
		explosion_buffer.loadFromFile("ResourceFiles/Audio/ExplosionSound.wav");
		ending_buffer.loadFromFile("ResourceFiles/Audio/EndingSound.wav");

		opening_song.setBuffer(opening_song_buffer);
		explosion_sound.setBuffer(explosion_buffer);
		ending_sound.setBuffer(ending_buffer);

		start_rect.setPosition(128 * 4 - 150, 96 * 4 - 50);
		start_rect.setOutlineColor(sf::Color::Black);
		start_rect.setOutlineThickness(4);
		start_text_drawer.text_scale = 5;
		start_text_drawer.x = 128 * 4 - 150 + 51;
		start_text_drawer.y = 96 * 4 - 50 + 44;

		ending_rect.setOutlineColor(sf::Color::Black);
		ending_rect.setOutlineThickness(4);
		dialog.typing_interval = 6;

		main_character_walk.animation_interval = 17;
		main_character_walk.scale = 3;
		main_character_walk.y = 546;
		main_character_still.scale = 3;
		main_character_still.x = 200;
		main_character_still.y = 546;
		ufo_spin.scale = 4;
		ufo_spin.x = 800;
		ufo_spin.animation_interval = 10;

		text_drawer.x = 12;
		text_drawer.y = 6;

		true_t.scale = 3;
		true_t.x = 12;
		true_t.y = 32;

		toenail_t.scale = 3;
		toenail_t.x = 48;
		toenail_t.y = 32;

		hamburger_t.scale = 3;
		hamburger_t.x = 84;
		hamburger_t.y = 32;
	}

	if (game_started_frame == -1 && game_started == false)
	{
		background_drawer.drawBackground(window);
		window->draw(start_rect);
		start_text_drawer.drawText(window, "start game", 0);

		if (
			sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
			checkIfInRectLogic(sf::Mouse::getPosition(*window), 128 * 4 - 150, 96 * 4 - 50, 128 * 4 - 150 + 300, 96 * 4 - 50 + 100)
			)
		{
			game_started_frame = frame;
			opening_song.setVolume(70);
			opening_song.play();
		}
	}

	if (game_started_frame != -1 && game_started == false)
	{
		background_drawer.drawBackground(window);
		text_drawer.drawText(window, "ENDINGS", 0);
		ending_rect.setPosition(8, 28);
		window->draw(ending_rect);
		ending_rect.setPosition(44, 28);
		window->draw(ending_rect);
		ending_rect.setPosition(80, 28);
		window->draw(ending_rect);
		if (frame >= game_started_frame + 184)
		{
			game_started = true;
			game_started_frame = -1;
			*frame_p = 0;
			frame = 0;
		}
	}

	if (game_started == false) { return; }

	if (ending_frame == -1)
	{
		background_drawer.drawBackground(window);

		if (frame > 264)
		{
			main_character_still.draw(window, frame);
		}
		if (frame > 1550)
		{
			ufo_spin.draw(window, frame);
		}

		if (response == -1)
		{
			selection_box.updateSelection(window);
		}
	
		selection_box.drawSelectionBox(window, frame);

		text_drawer.drawText(window, "ENDINGS", 0);
		ending_rect.setPosition(8, 28);
		window->draw(ending_rect);
		ending_rect.setPosition(44, 28);
		window->draw(ending_rect);
		ending_rect.setPosition(80, 28);
		window->draw(ending_rect);

		if (true_ending)
		{
			true_t.draw(window, frame);
		}

		if (toenail_ending)
		{
			toenail_t.draw(window, frame);
		}

		if (hamburger_ending)
		{
			hamburger_t.draw(window, frame);
		}
	}
	dialog.drawDialog(window, frame);


	if (selection_box.clicking && response == -1)
	{
		response = selection_box.selected;
		response_frame = frame;
	}

	if (frame <= 264)
	{
		main_character_walk.x = frame - 64;
		main_character_walk.draw(window, frame + 20);
	}
	else if (last_frame < 300 && frame >= 300)
	{
		dialog.active = true;
		dialog.setNewDialog("MainCharacter", "So tired...", frame);
	}
	else if (last_frame < 500 && frame >= 500)
	{
		dialog.setNewDialog("MainCharacter", "So hot...", frame);
	}
	else if (last_frame < 700 && frame >= 700)
	{
		dialog.setNewDialog("MainCharacter", "I wish the sun would just disappear...", frame);
	}
	else if (last_frame < 1075 && frame >= 1075)
	{
		dialog.active = false;
	}
	else if (frame < 1100) {}
	else if (frame <= 1142)
	{
		background_drawer.setState((frame - 1100) / 6);
	}
	else if (frame < 1148) {}
	else if (frame <= 1190)
	{
		background_drawer.setState((42 - (frame - 1148)) / 6);
	}
	else if (frame < 1250) {}
	else if (frame <= 1550)
	{
		ufo_spin.y = frame - 1250 - 50;
		ufo_spin.draw(window, frame);
	}
	else if (frame == 1551)
	{
		background_drawer.setState(0);
	}

	else if (last_frame < 1600 && frame >= 1600)
	{
		dialog.typing_interval = 3;
		dialog.active = true;
		dialog.show_next = false;
		dialog.setNewDialog("Alien", "Hey human is it cool if we destroy your\nentire planet", frame);
		dialog.language_type = 1;
	}
	else if (last_frame < 1890 && frame >= 1890)
	{
		ints.clear();
		ints = {2, 1, 1, 1};
		selection_box.setNewSelections("what the heck oh my lord oh my jesus christ is that\nan alien holy crap\ndid the sun just blink?\nyes.\nno.", 4, ints, frame);
		selection_box.active = true;
		answer_state = 1;
	}
	// Done
	else if (answer_state == 1)
	{
		if (response_frame != -1 && last_frame < response_frame + 20 && frame >= response_frame + 20)
		{
			dialog.setNewDialog("Alien", "TRANSLATION\nHey human is it cool if we destroy your\nentire planet?", frame);
			dialog.language_type = 0;
			dialog.show_next = true;
		}

		if (skip_num == 1 && dialog.skipped)
		{
			answer_state = next_answer_state;
			response = -1;
			skip_num = 2;
		}

		if (skip_num == 0 && dialog.skipped)
		{
			switch (response)
			{
			case 0:
				dialog.setNewDialog("MainCharacter", "what the heck oh my lord oh my jesus christ\nis that an alien holy crap", frame);
				next_answer_state = 2;
				break;
			case 1:
				dialog.setNewDialog("MainCharacter", "did the sun just blink?", frame);
				next_answer_state = 3;
				break;
			case 2:
				dialog.setNewDialog("MainCharacter", "yes.", frame);
				next_answer_state = 4;
				break;
			case 3:
				dialog.setNewDialog("MainCharacter", "no.", frame);
				next_answer_state = 5;
				break;
			}

			selection_box.active = false;
			skip_num = 1;
			response_frame = -1;
		}
	}
	
	// Done
	else if (answer_state == 2)
	{
		std::string alien_says = "Well from our perspective your the alien.\nbut are you saying we cant destroy your\nentire planet?";
		if (talking_frame != -1 && frame >= talking_frame + 450)
		{
			ints.clear();
			ints = { 2, 1, 1 };
			selection_box.setNewSelections("oh my god it really is an alien holy crap god\nalmighty what the heck\nyes.\nno.", 3, ints, frame);
			selection_box.active = true;
			talking_frame = -1;
		}

		if (response_frame != -1 && last_frame < response_frame + 20 && frame >= response_frame + 20)
		{
			dialog.setNewDialog("Alien", "TRANSLATION\n" + alien_says, frame);
			dialog.language_type = 0;
			dialog.show_next = true;
		}

		if (skip_num == 1 && dialog.skipped)
		{
			answer_state = next_answer_state;
			response = -1;
			skip_num = 2;
		}

		if (skip_num == 0 && dialog.skipped)
		{
			switch (response)
			{
			case 0:
				dialog.setNewDialog("MainCharacter", "oh my god it really is an alien holy crap god\nalmighty what the heck", frame);
				next_answer_state = 6;
				break;
			case 1:
				dialog.setNewDialog("MainCharacter", "yes.", frame);
				next_answer_state = 5;
				break;
			case 2:
				dialog.setNewDialog("MainCharacter", "no.", frame);
				next_answer_state = 4;
				break;
			}

			selection_box.active = false;
			skip_num = 1;
			response_frame = -1;
		}

		if (skip_num == 2 && answer_state == 2)
		{
			dialog.setNewDialog("Alien", alien_says, frame);
			dialog.language_type = 1;
			dialog.show_next = false;
			skip_num = 0;
			talking_frame = frame;
		}
	}
	
	// Done
	else if (answer_state == 3)
	{
		std::string alien_says = "sorry about that we were testing out\nour portable dyson spheres. now back on\ntopic can we destroy your planet?";
		if (talking_frame != -1 && frame >= talking_frame + 550)
		{
			ints.clear();
			ints = { 1, 1 };
			selection_box.setNewSelections("yes.\nno.", 2, ints, frame);
			selection_box.active = true;
			talking_frame = -1;
		}

		if (response_frame != -1 && last_frame < response_frame + 20 && frame >= response_frame + 20)
		{
			dialog.setNewDialog("Alien", "TRANSLATION\n" + alien_says, frame);
			dialog.language_type = 0;
			dialog.show_next = true;
		}

		if (skip_num == 1 && dialog.skipped)
		{
			answer_state = next_answer_state;
			response = -1;
			skip_num = 2;
		}

		if (skip_num == 0 && dialog.skipped)
		{

			switch (response)
			{
			case 0:
				dialog.setNewDialog("MainCharacter", "yes.", frame);
				next_answer_state = 4;
				break;
			case 1:
				dialog.setNewDialog("MainCharacter", "no.", frame);
				next_answer_state = 5;
				break;
			}

			selection_box.active = false;
			skip_num = 1;
			response_frame = -1;
		}

		if (skip_num == 2 && answer_state == 3)
		{
			dialog.setNewDialog("Alien", alien_says, frame);
			dialog.language_type = 1;
			dialog.show_next = false;
			skip_num = 0;
			talking_frame = frame;
		}
	}
	
	// DEATH
	else if (answer_state == 4)
	{
		if (skip_num == 2)
		{
			dialog.setNewDialog("Alien", "TRANSLATED\nalright cool thanks", frame);
			dialog.language_type = 0;
			dialog.show_next = true;
			skip_num = 0;
			talking_frame = -1;
			response = -1;
			response_frame = -1;
			next_answer_state = -1;
		}
		
		if (dialog.skipped && ending_frame != -1)
		{
			dialog.active = false;
			ending_frame = -1;
			*frame_p = 1551;
		}
		else if (dialog.skipped && ending_frame == -1)
		{
			ending_frame = frame;
			dialog.active = false;
			explosion_sound.play();
		}

		if (ending_frame != -1)
		{
			if (frame >= ending_frame + 100 && last_frame < ending_frame + 100)
			{
				dialog.active = true;
				dialog.setNewDialog("EMPTY", "The Earth was blown to smithereens\nby the alien spaceship.              \nno survivors.\n              restarting game...", frame);
			}
		}
	}
	
	// Done
	// CORRECT OPTION ONE
	else if (answer_state == 5)
	{
		std::string alien_says = "but why?";
		if (talking_frame != -1 && frame >= talking_frame + 80)
		{
			ints.clear();
			ints = { 1, 1, 1, 1, 1 };
			selection_box.setNewSelections("nevermind go ahead.\nbecause its evil.\ndestroying the earth is heroic.\nyes.\nno.", 5, ints, frame);
			selection_box.active = true;
			talking_frame = -1;
		}

		if (response_frame != -1 && last_frame < response_frame + 20 && frame >= response_frame + 20)
		{
			dialog.setNewDialog("Alien", "TRANSLATION\n" + alien_says, frame);
			dialog.language_type = 0;
			dialog.show_next = true;
		}

		if (skip_num == 1 && dialog.skipped)
		{
			answer_state = next_answer_state;
			response = -1;
			skip_num = 2;
		}

		if (skip_num == 0 && dialog.skipped)
		{
			switch (response)
			{
			case 0:
				dialog.setNewDialog("MainCharacter", "nevermind go ahead.", frame);
				next_answer_state = 4;
				break;
			case 1:
				dialog.setNewDialog("MainCharacter", "because its evil.", frame);
				next_answer_state = 7;
				break;
			case 2:
				dialog.setNewDialog("MainCharacter", "destroying the earth is heroic.", frame);
				next_answer_state = 9;
				break;
			case 3:
				dialog.setNewDialog("MainCharacter", "yes.", frame);
				next_answer_state = 8;
				break;
			case 4:
				dialog.setNewDialog("MainCharacter", "no.", frame);
				next_answer_state = 8;
				break;
			}

			selection_box.active = false;
			skip_num = 1;
			response_frame = -1;
		}

		if (skip_num == 2 && answer_state == 5)
		{
			dialog.setNewDialog("Alien", alien_says, frame);
			dialog.language_type = 1;
			dialog.show_next = false;
			skip_num = 0;
			talking_frame = frame;
		}
	}
	
	// DEATH
	else if (answer_state == 6)
	{
		if (skip_num == 2)
		{
			dialog.setNewDialog("Alien", "TRANSLATED\nalright im getting sick and tired of this.\nim doing it.", frame);
			dialog.language_type = 0;
			dialog.show_next = true;
			skip_num = 0;
			talking_frame = -1;
			response = -1;
			response_frame = -1;
			next_answer_state = -1;
		}

		if (dialog.skipped && ending_frame != -1)
		{
			dialog.active = false;
			ending_frame = -1;
			*frame_p = 1551;
		}
		else if (dialog.skipped && ending_frame == -1)
		{
			ending_frame = frame;
			dialog.active = false;
			explosion_sound.play();
		}

		if (ending_frame != -1)
		{
			if (frame >= ending_frame + 100 && last_frame < ending_frame + 100)
			{
				dialog.active = true;
				dialog.setNewDialog("EMPTY", "The Earth was violently blown to\nsmithereens by the alien spaceship.              \nno survivors.\n              restarting game...", frame);
			}
		}
	}
	
	// DEATH
	else if (answer_state == 7)
	{
		if (skip_num == 2)
		{
			dialog.setNewDialog("Alien", "TRANSLATED\nits evil? alright nice that is exactly what\ni was going for.", frame);
			dialog.language_type = 0;
			dialog.show_next = true;
			skip_num = 0;
			talking_frame = -1;
			response = -1;
			response_frame = -1;
			next_answer_state = -1;
		}

		if (dialog.skipped && ending_frame != -1)
		{
			dialog.active = false;
			ending_frame = -1;
			*frame_p = 1551;
		}
		else if (dialog.skipped && ending_frame == -1)
		{
			ending_frame = frame;
			dialog.active = false;
			explosion_sound.play();
		}

		if (ending_frame != -1)
		{
			if (frame >= ending_frame + 100 && last_frame < ending_frame + 100)
			{
				dialog.active = true;
				dialog.setNewDialog("EMPTY", "The Earth was blown to smithereens\nby the alien spaceship.              \nno survivors.\n              restarting game...", frame);
			}
		}
	}
	
	// Done
	else if (answer_state == 8)
	{
		std::string alien_says = "what is your favorite food?";
		if (talking_frame != -1 && frame >= talking_frame + 200)
		{
			ints.clear();
			ints = { 3, 1 };
			selection_box.setNewSelections("My apologies for the incorrect response. I did not\nknow what you said and thus did not know if that\nwas or was not an appropriate reply.\nhamburger.", 2, ints, frame);
			selection_box.active = true;
			talking_frame = -1;
		}

		if (response_frame != -1 && last_frame < response_frame + 20 && frame >= response_frame + 20)
		{
			dialog.setNewDialog("Alien", "TRANSLATION\n" + alien_says, frame);
			dialog.language_type = 0;
			dialog.show_next = true;
		}

		if (skip_num == 1 && dialog.skipped)
		{
			answer_state = next_answer_state;
			response = -1;
			skip_num = 2;
		}

		if (skip_num == 0 && dialog.skipped)
		{
			switch (response)
			{
			case 0:
				dialog.setNewDialog("MainCharacter", "My apologies for the incorrect response. I\ndid not know what you said and thus did not\nknow if that was or was not an appropriate\nreply.", frame);
				next_answer_state = 10;
				break;
			case 1:
				dialog.setNewDialog("MainCharacter", "hamburger.", frame);
				next_answer_state = 11;
				break;
			
			}

			selection_box.active = false;
			skip_num = 1;
			response_frame = -1;
		}

		if (skip_num == 2 && answer_state == 8)
		{
			dialog.setNewDialog("Alien", alien_says, frame);
			dialog.language_type = 1;
			dialog.show_next = false;
			skip_num = 0;
			talking_frame = frame;
		}
	}
	
	// DONE
	// CORRECT OPTION TWO
	else if (answer_state == 9)
	{
		std::string alien_says = "heroic? no im trying to be evil. nevermind.";
		if (talking_frame != -1 && frame >= talking_frame + 200)
		{
			ints.clear();
			ints = { 2, 2, 1 };
			selection_box.setNewSelections("if you have some free time do you mind making me\na few pyramids?\njust because it is heroic doesnt mean you should do\nit.\ngood.", 3, ints, frame);
			selection_box.active = true;
			talking_frame = -1;
		}

		if (response_frame != -1 && last_frame < response_frame + 20 && frame >= response_frame + 20)
		{
			dialog.setNewDialog("Alien", "TRANSLATION\n" + alien_says, frame);
			dialog.language_type = 0;
			dialog.show_next = true;
		}

		if (skip_num == 1 && dialog.skipped)
		{
			answer_state = next_answer_state;
			response = -1;
			skip_num = 2;
		}

		if (skip_num == 0 && dialog.skipped)
		{
			switch (response)
			{
			case 0:
				dialog.setNewDialog("MainCharacter", "if you have some free time do you mind\nmaking me a few pyramids?", frame);
				next_answer_state = 12;
				break;
			case 1:
				dialog.setNewDialog("MainCharacter", "just because it is heroic doesnt mean you\nshould do it.", frame);
				next_answer_state = 13;
				break;
			case 2:
				dialog.setNewDialog("MainCharacter", "good.", frame);
				next_answer_state = 13;
				break;
			}

			selection_box.active = false;
			skip_num = 1;
			response_frame = -1;
		}

		if (skip_num == 2 && answer_state == 9)
		{
			dialog.setNewDialog("Alien", alien_says, frame);
			dialog.language_type = 1;
			dialog.show_next = false;
			skip_num = 0;
			talking_frame = frame;
		}
	}
	
	// DEATH
	else if (answer_state == 10)
	{
		if (skip_num == 2)
		{
			dialog.setNewDialog("Alien", "TRANSLATED\nHow dare you not answer my question.\nThats it im blowing this planet up.", frame);
			dialog.language_type = 0;
			dialog.show_next = true;
			skip_num = 0;
			talking_frame = -1;
			response = -1;
			response_frame = -1;
			next_answer_state = -1;
		}

		if (dialog.skipped && ending_frame != -1)
		{
			dialog.active = false;
			ending_frame = -1;
			*frame_p = 1551;
		}
		else if (dialog.skipped && ending_frame == -1)
		{
			ending_frame = frame;
			dialog.active = false;
			explosion_sound.play();
		}

		if (ending_frame != -1)
		{
			if (frame >= ending_frame + 100 && last_frame < ending_frame + 100)
			{
				dialog.active = true;
				dialog.setNewDialog("EMPTY", "The Earth was blown to smithereens\nby the alien spaceship.              \nno survivors.\n              restarting game...", frame);
			}
		}
	}
	
	// HAMBURGER ENDING
	else if (answer_state == 11)
	{
		if (skip_num == 2)
		{
			dialog.setNewDialog("Alien", "TRANSLATED\nomg me too. okay ill spare you i guess.\nbye.", frame);
			dialog.language_type = 0;
			dialog.show_next = true;
			skip_num = 0;
			talking_frame = -1;
			response = -1;
			response_frame = -1;
			next_answer_state = -1;
		}

		if (dialog.skipped && ending_frame != -1)
		{
			dialog.active = false;
			ending_frame = -1;
			*frame_p = 1551;
			hamburger_ending = true;
		}
		else if (dialog.skipped && ending_frame == -1)
		{
			ending_frame = frame;
			dialog.active = false;
			ending_sound.play();
		}

		if (ending_frame != -1)
		{
			if (frame >= ending_frame + 100 && last_frame < ending_frame + 100)
			{
				dialog.active = true;
				dialog.setNewDialog("EMPTY", "HAMBGURGER ENDING              \nThe alien left and never returned after\nrealizing your similarities.\n              restarting game...", frame);
			}
		}
	}
	
	// DONE
	// CORRECT OPTION THREE
	else if (answer_state == 12)
	{
		std::string alien_says = "sure here you go i guess. but now i need a\nfavor from you.";
		if (talking_frame != -1 && frame >= talking_frame + 320)
		{
			ints.clear();
			ints = { 1, 1, 1, 1, 1 };
			selection_box.setNewSelections("wow thanks man.\nnot big enough.\nokay.\nno.\nyou look kind of gross.", 5, ints, frame);
			selection_box.active = true;
			talking_frame = -1;
		}

		if (response_frame != -1 && last_frame < response_frame + 20 && frame >= response_frame + 20)
		{
			dialog.setNewDialog("Alien", "TRANSLATION\n" + alien_says, frame);
			dialog.language_type = 0;
			dialog.show_next = true;
		}

		if (skip_num == 1 && dialog.skipped)
		{
			answer_state = next_answer_state;
			response = -1;
			skip_num = 2;
		}

		if (skip_num == 0 && dialog.skipped)
		{
			switch (response)
			{
			case 0:
				dialog.setNewDialog("MainCharacter", "wow thanks man.", frame);
				next_answer_state = 16;
				break;
			case 1:
				dialog.setNewDialog("MainCharacter", "not big enough.", frame);
				next_answer_state = 15;
				break;
			case 2:
				dialog.setNewDialog("MainCharacter", "okay.", frame);
				next_answer_state = 16;
				break;
			case 3:
				dialog.setNewDialog("MainCharacter", "no.", frame);
				next_answer_state = 14;
				break;
			case 4:
				dialog.setNewDialog("MainCharacter", "you look kind of gross", frame);
				next_answer_state = 14;
				break;
			}

			selection_box.active = false;
			skip_num = 1;
			response_frame = -1;
		}

		if (skip_num == 2 && answer_state == 12)
		{
			background_drawer.setState(8);
			dialog.setNewDialog("Alien", alien_says, frame);
			dialog.language_type = 1;
			dialog.show_next = false;
			skip_num = 0;
			talking_frame = frame;
		}
	}

	// DEATH
	else if (answer_state == 13)
	{
		if (skip_num == 2)
		{
			dialog.setNewDialog("Alien", "TRANSLATED\nyour being mean now. i already said i wasnt\ngonna do it.         \nyou know what nevermind.", frame);
			dialog.language_type = 0;
			dialog.show_next = true;
			skip_num = 0;
			talking_frame = -1;
			response = -1;
			response_frame = -1;
			next_answer_state = -1;
		}

		if (dialog.skipped && ending_frame != -1)
		{
			dialog.active = false;
			ending_frame = -1;
			*frame_p = 1551;
		}
		else if (dialog.skipped && ending_frame == -1)
		{
			ending_frame = frame;
			dialog.active = false;
			explosion_sound.play();
		}

		if (ending_frame != -1)
		{
			if (frame >= ending_frame + 100 && last_frame < ending_frame + 100)
			{
				dialog.active = true;
				dialog.setNewDialog("EMPTY", "The Earth was blown to smithereens\nby the alien spaceship.              \nno survivors.\n              restarting game...", frame);
			}
		}
	}
	
	else if (answer_state == 14)
	{
		std::string alien_says = "ok fine you are not getting any pyramids.";
		if (talking_frame != -1 && frame >= talking_frame + 280)
		{
			ints.clear();
			ints = { 1, 1, 1};
			selection_box.setNewSelections("sorry i take it back\ni hate you\nok fine", 3, ints, frame);
			selection_box.active = true;
			talking_frame = -1;
		}

		if (response_frame != -1 && last_frame < response_frame + 20 && frame >= response_frame + 20)
		{
			dialog.setNewDialog("Alien", "TRANSLATION\n" + alien_says, frame);
			dialog.language_type = 0;
			dialog.show_next = true;
		}

		if (skip_num == 1 && dialog.skipped)
		{
			answer_state = next_answer_state;
			response = -1;
			skip_num = 2;
		}

		if (skip_num == 0 && dialog.skipped)
		{
			switch (response)
			{
			case 0:
				dialog.setNewDialog("MainCharacter", "sorry i take it back.", frame);
				next_answer_state = 16;
				break;
			case 1:
				dialog.setNewDialog("MainCharacter", "i hate you.", frame);
				next_answer_state = 18;
				break;
			case 2:
				dialog.setNewDialog("MainCharacter", "okay fine.", frame);
				next_answer_state = 18;
				break;
			}

			selection_box.active = false;
			skip_num = 1;
			response_frame = -1;
		}

		if (skip_num == 2 && answer_state == 14)
		{
			dialog.setNewDialog("Alien", alien_says, frame);
			dialog.language_type = 1;
			dialog.show_next = false;
			skip_num = 0;
			talking_frame = frame;
		}
	}
	
	else if (answer_state == 15)
	{
		std::string alien_says = "ok fine but im gonna need a bigger favor.\nI need your toenails...";
		if (talking_frame != -1 && frame >= talking_frame + 280)
		{
			ints.clear();
			ints = { 1, 1, 1 };
			selection_box.setNewSelections("not. big. enough.\ndeal.\nokay man that is kind of weird.", 3, ints, frame);
			selection_box.active = true;
			talking_frame = -1;
		}

		if (response_frame != -1 && last_frame < response_frame + 20 && frame >= response_frame + 20)
		{
			dialog.setNewDialog("Alien", "TRANSLATION\n" + alien_says, frame);
			dialog.language_type = 0;
			dialog.show_next = true;
		}

		if (skip_num == 1 && dialog.skipped)
		{
			answer_state = next_answer_state;
			response = -1;
			skip_num = 2;
		}

		if (skip_num == 0 && dialog.skipped)
		{
			switch (response)
			{
			case 0:
				dialog.setNewDialog("MainCharacter", "not. big. enough.", frame);
				next_answer_state = 6;
				break;
			case 1:
				dialog.setNewDialog("MainCharacter", "deal.", frame);
				next_answer_state = 19;
				break;
			case 2:
				dialog.setNewDialog("MainCharacter", "okay man that is kind of weird.", frame);
				next_answer_state = 17;
				break;
			}

			selection_box.active = false;
			skip_num = 1;
			response_frame = -1;
		}

		if (skip_num == 2 && answer_state == 15)
		{
			background_drawer.setState(9);
			dialog.setNewDialog("Alien", alien_says, frame);
			dialog.language_type = 1;
			dialog.show_next = false;
			skip_num = 0;
			talking_frame = frame;
		}
	}
	
	// CORRECT ANSWER THREE
	else if (answer_state == 16)
	{
		std::string alien_says = "alright. the deal is that you cannot let\nanyone know that i was here. as far as\neveryone knows you made these pyramids.";
		if (talking_frame != -1 && frame >= talking_frame + 450)
		{
			ints.clear();
			ints = { 1 };
			selection_box.setNewSelections("deal.", 1, ints, frame);
			selection_box.active = true;
			talking_frame = -1;
		}

		if (response_frame != -1 && last_frame < response_frame + 20 && frame >= response_frame + 20)
		{
			dialog.setNewDialog("Alien", "TRANSLATION\n" + alien_says, frame);
			dialog.language_type = 0;
			dialog.show_next = true;
		}

		if (skip_num == 1 && dialog.skipped)
		{
			answer_state = next_answer_state;
			response = -1;
			skip_num = 2;
		}

		if (skip_num == 0 && dialog.skipped)
		{
			switch (response)
			{
			case 0:
				dialog.setNewDialog("MainCharacter", "deal.", frame);
				next_answer_state = 20;
				break;
			}

			selection_box.active = false;
			skip_num = 1;
			response_frame = -1;
		}

		if (skip_num == 2 && answer_state == 16)
		{
			dialog.setNewDialog("Alien", alien_says, frame);
			dialog.language_type = 1;
			dialog.show_next = false;
			skip_num = 0;
			talking_frame = frame;
		}
	}

	// DEATH
	else if (answer_state == 17)
	{
		if (skip_num == 2)
		{
			dialog.setNewDialog("Alien", "TRANSLATED\nshut up your weird", frame);
			dialog.language_type = 0;
			dialog.show_next = true;
			skip_num = 0;
			talking_frame = -1;
			response = -1;
			response_frame = -1;
			next_answer_state = -1;
		}

		if (dialog.skipped && ending_frame != -1)
		{
			dialog.active = false;
			ending_frame = -1;
			*frame_p = 1551;
		}
		else if (dialog.skipped && ending_frame == -1)
		{
			ending_frame = frame;
			dialog.active = false;
			explosion_sound.play();
		}

		if (ending_frame != -1)
		{
			if (frame >= ending_frame + 100 && last_frame < ending_frame + 100)
			{
				dialog.active = true;
				dialog.setNewDialog("EMPTY", "The Earth was blown to smithereens\nby the alien spaceship.              \nno survivors.\n              restarting game...", frame);
			}
		}
	}

	// DEATH
	else if (answer_state == 18)
	{
		if (skip_num == 2)
		{
			dialog.setNewDialog("Alien", "TRANSLATED\nforget it. humans are stupid.\ngoodbye.", frame);
			dialog.language_type = 0;
			dialog.show_next = true;
			skip_num = 0;
			talking_frame = -1;
			response = -1;
			response_frame = -1;
			next_answer_state = -1;
		}

		if (dialog.skipped && ending_frame != -1)
		{
			dialog.active = false;
			ending_frame = -1;
			*frame_p = 1551;
		}
		else if (dialog.skipped && ending_frame == -1)
		{
			ending_frame = frame;
			dialog.active = false;
			explosion_sound.play();
		}

		if (ending_frame != -1)
		{
			if (frame >= ending_frame + 100 && last_frame < ending_frame + 100)
			{
				dialog.active = true;
				dialog.setNewDialog("EMPTY", "The Earth was blown to smithereens\nby the alien spaceship.              \nno survivors.\n              restarting game...", frame);
			}
		}
	}

	// TOENAIL ENDING
	else if (answer_state == 19)
	{
		if (skip_num == 2)
		{
			dialog.setNewDialog("Alien", "TRANSLATED\nyummy toenails", frame);
			dialog.language_type = 0;
			dialog.show_next = true;
			skip_num = 0;
			talking_frame = -1;
			response = -1;
			response_frame = -1;
			next_answer_state = -1;
		}

		if (dialog.skipped && ending_frame != -1)
		{
			dialog.active = false;
			ending_frame = -1;
			*frame_p = 1551;
			toenail_ending = true;
		}
		else if (dialog.skipped && ending_frame == -1)
		{
			ending_frame = frame;
			dialog.active = false;
			ending_sound.play();
		}

		if (ending_frame != -1)
		{
			if (frame >= ending_frame + 100 && last_frame < ending_frame + 100)
			{
				dialog.active = true;
				dialog.setNewDialog("EMPTY", "TOENAIL ENDING    \nyour toenails were violently ripped off and\nyou died.   \n              restarting game...", frame);
			}
		}
	}

	// TRUE ENDING
	else if (answer_state == 20)
	{
		if (skip_num == 2)
		{
			dialog.setNewDialog("Alien", "TRANSLATED\nfarewell. human...", frame);
			dialog.language_type = 0;
			dialog.show_next = true;
			skip_num = 0;
			talking_frame = -1;
			response = -1;
			response_frame = -1;
			next_answer_state = -1;
		}

		if (dialog.skipped && ending_frame != -1)
		{
			dialog.active = false;
			ending_frame = -1;
			*frame_p = 1551;
			true_ending = true;
		}
		else if (dialog.skipped && ending_frame == -1)
		{
			ending_frame = frame;
			dialog.active = false;
			ending_sound.play();
		}

		if (ending_frame != -1)
		{
			if (frame >= ending_frame + 100 && last_frame < ending_frame + 100)
			{
				dialog.active = true;
				dialog.setNewDialog("EMPTY", "TRUE ENDING              \nThe alien left Earth and never returned\nafter making that deal.\n              restarting game...", frame);
			}
		}
	}

	last_frame = frame;
}