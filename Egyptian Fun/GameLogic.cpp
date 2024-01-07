#include "GameLogic.h"

GameLogic::GameLogic()
{
}


//---------// HARD CODED GAME LOGIC //---------//

void GameLogic::updateGame(sf::RenderWindow* window, double* frame_p)
{
	int frame = (int)*frame_p;

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
	std::vector<int> ints;

	calls++;

	if (calls == 1)
	{
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
	}

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
		dialog.typing_interval = 4;
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
		
		if (dialog.skipped)
		{
			*frame_p = 1551;
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

		if (dialog.skipped)
		{
			*frame_p = 1551;
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

		if (dialog.skipped)
		{
			*frame_p = 1551;
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

		if (dialog.skipped)
		{
			*frame_p = 1551;
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

		if (dialog.skipped)
		{
			*frame_p = 1551;
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
			dialog.setNewDialog("Alien", "TRANSLATED\nyour being mean now. i already said i wasnt\ngonna do it. you know what. maybe i will do\nit just because you are being so mean. ", frame);
			dialog.language_type = 0;
			dialog.show_next = true;
			skip_num = 0;
			talking_frame = -1;
			response = -1;
			response_frame = -1;
			next_answer_state = -1;
		}

		if (dialog.skipped)
		{
			*frame_p = 1551;
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
		if (talking_frame != -1 && frame >= talking_frame + 350)
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

		if (dialog.skipped)
		{
			*frame_p = 1551;
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

		if (dialog.skipped)
		{
			*frame_p = 1551;
		}
	}

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

		if (dialog.skipped)
		{
			*frame_p = 1551;
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

		if (dialog.skipped)
		{
			*frame_p = 1551;
		}
	}

	last_frame = frame;
}