#include "gaster.h"

Gaster::Gaster(Player *player_instance, sf::RenderWindow *window_instance) : text_position(0), text_tick(0), text_duration_tick(0)
{
	FileManager::LoadFromFile(this->gaster_texture, "bin\\sprites\\gaster.png");
	FileManager::LoadFromFile(this->gaster_surprised_texture, "bin\\sprites\\gaster_2.png");
	FileManager::LoadFromFile(this->blaster_texture[0], "bin\\sprites\\blaster_default.png");
	FileManager::LoadFromFile(this->blaster_texture[1], "bin\\sprites\\blaster_blue.png");
	FileManager::LoadFromFile(this->blaster_texture[2], "bin\\sprites\\blaster_orange.png");
	FileManager::LoadFromFile(this->blaster_firing_texture[0], "bin\\sprites\\blaster_fire_1.png");
	FileManager::LoadFromFile(this->blaster_firing_texture[1], "bin\\sprites\\blaster_fire_2.png");
	FileManager::LoadFromFile(this->blaster_laser_texture[0], "bin\\sprites\\blaster_default_laser.png");
	FileManager::LoadFromFile(this->blaster_laser_texture[1], "bin\\sprites\\blaster_blue_laser.png");
	FileManager::LoadFromFile(this->blaster_laser_texture[2], "bin\\sprites\\blaster_orange_laser.png");
	FileManager::LoadFromFile(this->speech_bubble_texture, "bin\\sprites\\speech_bubble.png");
	FileManager::LoadFromFile(this->dialog_font, "bin\\fonts\\determination_mono.ttf");
	FileManager::LoadFromFile(this->blaster_intro, "bin\\audio\\blaster_intro.wav");
	FileManager::LoadFromFile(this->blaster_fire, "bin\\audio\\blaster_fire.wav");

	if (!FileManager::IsAnyFileMissing())
	{
		this->player_instance = player_instance;
		this->window_instance = window_instance;
		this->speech_bubble_sprite.setTexture(this->speech_bubble_texture);
		this->speech_bubble_sprite.setPosition(58.f, 30.f);
		this->gaster_sprite.setTexture(this->gaster_texture);
		this->gaster_sprite.setPosition(sf::Vector2f(285.f, 11.f));
		this->gaster_sprite.setScale(sf::Vector2f(0.8f, 0.8f));
		this->dialog.setFont(this->dialog_font);
		this->dialog.setCharacterSize(15);
		this->dialog.setFillColor(sf::Color::Black);
		this->dialog.setPosition(70.f, 30.f);
	}
}

void Gaster::SetText(const std::string &text, const DWORD text_duration)
{
	this->text_duration_tick = GetTickCount() + text_duration;
	this->text_position = 0;
	this->dialog.setString("");
	this->dialog_text = text;
}

void Gaster::ToggleGasterSurprised(const bool toggle)
{
	if (toggle)
	{
		this->gaster_sprite.setTexture(this->gaster_surprised_texture);
	}
	else
	{
		this->gaster_sprite.setTexture(this->gaster_texture);
	}
}

void Gaster::Update()
{
	const DWORD current_tick = GetTickCount();

	if (this->text_duration_tick > current_tick)
	{
		if (this->text_position != this->dialog_text.size() && (current_tick > this->text_tick))
		{
			this->text_tick = current_tick + 100;
			this->dialog.setString(this->dialog.getString() + this->dialog_text[this->text_position]);
			this->text_position++;
		}
		this->window_instance->draw(this->speech_bubble_sprite);
		this->window_instance->draw(this->dialog);
	}

	this->window_instance->draw(this->gaster_sprite);

	if (!this->blasters.empty())
	{
		for (auto it = begin(this->blasters); it != end(this->blasters); ++it)
		{
			if (it->spawn_interval < current_tick)
			{
				this->window_instance->draw(it->blaster_sprite);

				if (!it->state_firing)
				{
					if (it->fire_tick < current_tick)
					{
						it->blaster_fire_sound.setBuffer(this->blaster_fire);
						it->blaster_fire_sound.play();
						it->blaster_sprite.setTexture(this->blaster_firing_texture[0]);
						it->state_firing = true;
						it->fire_tick = current_tick + it->fire_duration;
					}
					else
					{
						switch (it->direction)
						{
							case Blaster_Direction::Direction_Down:
							{
								if (it->blaster_sprite.getRotation() != 0)
								{
									it->blaster_sprite.rotate(5.0);
								}
								break;
							}
							case Blaster_Direction::Direction_Left:
							{
								if (it->blaster_sprite.getRotation() != 90)
								{
									it->blaster_sprite.rotate(5.0);
								}
								break;
							}
							case Blaster_Direction::Direction_Right:
							{
								if (it->blaster_sprite.getRotation() != 270)
								{
									it->blaster_sprite.rotate(-5.0);
								}
								break;
							}
						}

						if ((it->blaster_sprite.getColor().a + 10) < 255)
						{
							it->blaster_sprite.setColor(sf::Color(255, 255, 255, it->blaster_sprite.getColor().a + 10));
						}
						else
						{
							it->blaster_sprite.setColor(sf::Color(255, 255, 255, 255));
						}

						if (!it->blaster_intro)
						{
							it->blaster_intro = true;
							it->blaster_fire_sound.play();
						}
					}
				}
				else
				{
					if (current_tick > it->switch_blaster_sprite_tick)
					{
						it->switch_blaster_sprite_tick = current_tick + 50;

						if (it->switch_blaster_sprite)
						{
							it->switch_blaster_sprite = false;
							it->blaster_sprite.setTexture(this->blaster_firing_texture[0]);
						}
						else
						{
							it->switch_blaster_sprite = true;
							it->blaster_sprite.setTexture(this->blaster_firing_texture[1]);
						}
					}

					this->window_instance->draw(it->laser_sprite);

					if (current_tick < it->fire_tick)
					{
						if (this->player_instance->GetGlobalBounds().intersects(it->laser_sprite.getGlobalBounds()))
						{
							if (it->blaster_type == Blaster_Type::Default_Blaster || it->blaster_type == Blaster_Type::Blue_Blaster && this->player_instance->IsPlayerMoving() || it->blaster_type == Blaster_Type::Orange_Blaster && !this->player_instance->IsPlayerMoving())
							{
								this->player_instance->TakeDamage(15);
							}
						}
					}
					else
					{

						if (it->removal_tick < current_tick)
						{
							it = this->blasters.erase(it);
							it--;
						}
						else
						{
							if ((it->blaster_sprite.getColor().a - 10) > 0)
							{
								it->blaster_sprite.setColor(sf::Color(255, 255, 255, it->blaster_sprite.getColor().a - 10));
							}
							else
							{
								it->blaster_sprite.setColor(sf::Color(255, 255, 255, 0));
							}

							if ((it->laser_sprite.getColor().a - 30) > 0)
							{
								it->laser_sprite.setColor(sf::Color(255, 255, 255, it->laser_sprite.getColor().a - 30));
							}
							else
							{
								it->laser_sprite.setColor(sf::Color(255, 255, 255, 0));
							}
						}
					}
				}
			}
		}
	}
}

void Gaster::AddBlaster(const sf::Vector2f &position, const char blaster_direction, const DWORD spawn_interval, const char blaster_type, const DWORD fire_duration, const DWORD interval_before_firing)
{
	Blaster blaster;
	blaster.spawn_interval = GetTickCount() + spawn_interval;
	blaster.blaster_sprite.setPosition(position);
	blaster.state_firing = false;
	blaster.switch_blaster_sprite = false;
	blaster.switch_blaster_sprite_tick = 0;
	blaster.blaster_intro = false;
	blaster.blaster_type = blaster_type;
	blaster.direction = blaster_direction;
	blaster.fire_duration = fire_duration;
	blaster.removal_tick = GetTickCount() + 2100 + spawn_interval;

	switch (blaster_type)
	{
		case Blaster_Type::Default_Blaster:
		{
			blaster.blaster_sprite.setTexture(this->blaster_texture[0]);
			blaster.laser_sprite.setTexture(this->blaster_laser_texture[0]);
			break;
		}
		case Blaster_Type::Blue_Blaster:
		{
			blaster.blaster_sprite.setTexture(this->blaster_texture[1]);
			blaster.laser_sprite.setTexture(this->blaster_laser_texture[1]);
			break;
		}
		case Blaster_Type::Orange_Blaster:
		{
			blaster.blaster_sprite.setTexture(this->blaster_texture[2]);
			blaster.laser_sprite.setTexture(this->blaster_laser_texture[2]);
			break;
		}
	}

	switch (blaster_direction)
	{
		case Blaster_Direction::Direction_Down:
		{
			blaster.laser_sprite.setPosition(sf::Vector2f(position.x + 34.f, position.y - 11.f));
			blaster.blaster_sprite.setRotation(-90);
			break;
		}
		case Blaster_Direction::Direction_Left:
		{
			blaster.laser_sprite.setPosition(sf::Vector2f(position.x + 11.f, position.y + 34.f));
			blaster.blaster_sprite.setRotation(0);
			blaster.laser_sprite.setRotation(90);
			break;
		}
		case Blaster_Direction::Direction_Right:
		{
			blaster.laser_sprite.setPosition(sf::Vector2f(position.x - 11.f, position.y - 34.f));
			blaster.blaster_sprite.setRotation(0);
			blaster.laser_sprite.setRotation(270);
			break;
		}
	}

	blaster.fire_tick = GetTickCount() + interval_before_firing + spawn_interval;
	blaster.blaster_sprite.setColor(sf::Color(255, 255, 255, 50));
	blaster.blaster_fire_sound.setBuffer(this->blaster_intro);
	this->blasters.push_back(blaster);
}

void Gaster::Reset()
{
	this->gaster_sprite.setTexture(this->gaster_texture);
	this->text_position = 0;
	this->text_tick = 0;
	this->text_duration_tick = 0;
	this->blasters.clear();
}