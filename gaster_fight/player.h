#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

class Board;

class Player
{
	private:
		sf::Font player_name_font;
		sf::Text player_name_text;
		sf::Text player_health_text;
		sf::Texture player_texture;
		sf::Texture player_dead_texture;
		sf::Sprite player_sprite;
		sf::Texture health_texture;
		sf::Texture health_texture_cover;
		sf::Sprite health_sprite;
		sf::Sprite health_sprite_cover;
		sf::Texture miss_attack_texture;
		sf::Sprite miss_attack_sprite;
		std::array<sf::Texture, 3> attack_texture;
		sf::SoundBuffer damage_buffer;
		sf::SoundBuffer death_buffer;
		sf::SoundBuffer player_attack_buffer;
		sf::SoundBuffer player_heal_buffer;
		sf::Sound player_heal;
		sf::Sound player_attack;
		sf::Sound damage_sound;
		int health;
		DWORD damage_tick;
		DWORD use_button_tick;
		bool flash_damage_color;
		bool player_turn;
		bool is_moving;
		char button_pressed;
		char button_hovered;
		char heal_items_available;
		sf::RenderWindow *window_instance;
		Board *board_instance;

		void HoverButton(const char button_id);

	public:
		explicit Player(sf::RenderWindow *window_instance, Board *board_instance);
		
		inline sf::Vector2f GetPlayerPosition() const
		{
			return player_sprite.getPosition();
		}

		inline sf::FloatRect GetGlobalBounds() const
		{
			return player_sprite.getGlobalBounds();
		}

		inline int GetPlayerHealth() const
		{
			return health;
		}

		inline bool IsPlayerInTurn() const
		{
			return player_turn;
		}

		inline void SetPlayerPosition(const float x, const float y)
		{
			player_sprite.setPosition(sf::Vector2f(x, y));
		}

		inline DWORD GetButtonUseTick() const
		{
			return use_button_tick;
		}

		inline bool IsPlayerMoving() const
		{
			return is_moving;
		}

		void TogglePlayerTurn(const bool toggle, const std::string &board_text = "None", const DWORD board_text_delay = 0);
		void TakeDamage(const int amount);
		void Update();
		void Reset();
};

#endif