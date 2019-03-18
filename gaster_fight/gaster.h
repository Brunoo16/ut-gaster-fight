#ifndef _GASTER_H_
#define _GASTER_H_

#include "main.h"

class Player;

enum Blaster_Direction
{
	Direction_Down,
	Direction_Right,
	Direction_Left
};

enum Blaster_Type
{
	Default_Blaster,
	Blue_Blaster,
	Orange_Blaster
};

class Gaster
{
	private:
		sf::Font dialog_font;
		sf::Text dialog;
		std::string dialog_text;
		DWORD text_position;
		DWORD text_tick;
		DWORD text_duration_tick;
		sf::Texture gaster_texture;
		sf::Texture gaster_surprised_texture;
		sf::Sprite gaster_sprite;
		std::array<sf::Texture, 3> blaster_texture;
		std::array<sf::Texture, 2> blaster_firing_texture;
		std::array<sf::Texture, 3> blaster_laser_texture;
		sf::SoundBuffer blaster_intro;
		sf::SoundBuffer blaster_fire;
		sf::Texture speech_bubble_texture;
		sf::Sprite speech_bubble_sprite;

		struct Blaster
		{
			DWORD spawn_interval;
			sf::Sprite blaster_sprite;
			sf::Sprite laser_sprite;
			char direction;
			char blaster_type;
			bool state_firing;
			bool switch_blaster_sprite;
			bool blaster_intro;
			DWORD switch_blaster_sprite_tick;
			DWORD fire_tick;
			DWORD fire_duration;
			DWORD removal_tick;
			sf::Sound blaster_fire_sound;
		};

		std::deque<Blaster> blasters;

		Player *player_instance;
		sf::RenderWindow *window_instance;

	public:
		explicit Gaster(Player *player_instance, sf::RenderWindow *window_instance);

		void SetText(const std::string &text, const DWORD text_duration);
		void ToggleGasterSurprised(const bool toggle);
		void Update();
		void AddBlaster(const sf::Vector2f &position, const char blaster_direction, const DWORD spawn_interval = 0, const char blaster_type = Blaster_Type::Default_Blaster, const DWORD fire_duration = 1000, const DWORD interval_before_firing = 750);
		void Reset();
};

#endif 