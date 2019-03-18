#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

class Player;
class Gaster;
class Board;

class Game
{
	private:
		sf::RenderWindow window;
		sf::Image game_icon;
		sf::Music game_music;
		sf::Texture game_over_texture;
		sf::Sprite game_over_sprite;
		sf::Font credits_text_font;
		sf::Text credits_text;
		DWORD turn_tick;
		int current_turn;
		Board *board;
		Player *player;
		Gaster *gaster;

	public:
		Game();

		void Update();
};

#endif