#include "player.h"

Player::Player(sf::RenderWindow *window_instance, Board *board_instance) : health(100), damage_tick(0), use_button_tick(0)
{
	FileManager::LoadFromFile(this->player_texture, "bin\\sprites\\heart.png");
	FileManager::LoadFromFile(this->player_dead_texture, "bin\\sprites\\heart-broken.png");
	FileManager::LoadFromFile(this->health_texture, "bin\\sprites\\health_bar.png");
	FileManager::LoadFromFile(this->health_texture_cover, "bin\\sprites\\health_bar_cover.png");
	FileManager::LoadFromFile(this->miss_attack_texture, "bin\\sprites\\miss.png");
	FileManager::LoadFromFile(this->attack_texture[0], "bin\\sprites\\slice_1.png");
	FileManager::LoadFromFile(this->attack_texture[1], "bin\\sprites\\slice_2.png");
	FileManager::LoadFromFile(this->attack_texture[2], "bin\\sprites\\slice_3.png");
	FileManager::LoadFromFile(this->player_name_font, "bin\\fonts\\mars_needs_cunnilingus.ttf");
	FileManager::LoadFromFile(this->damage_buffer, "bin\\audio\\hurtsound.wav");
	FileManager::LoadFromFile(this->player_heal_buffer, "bin\\audio\\healsound.wav");
	FileManager::LoadFromFile(this->player_attack_buffer, "bin\\audio\\player_attack.wav");

	if (!FileManager::IsAnyFileMissing())
	{
		this->window_instance = window_instance;
		this->board_instance = board_instance;
		this->button_hovered = Button_Type::Fight_Button;
		this->button_pressed = Button_Type::None;
		this->flash_damage_color = false;
		this->player_turn = false;
		this->is_moving = false;
		this->heal_items_available = 1;
		this->player_name_text.setFont(this->player_name_font);
		this->player_name_text.setCharacterSize(23);
		this->player_name_text.setFillColor(sf::Color::White);
		this->player_name_text.setPosition(32.f, 394.f);
		this->player_name_text.setString("Chara   LV 20");
		this->player_health_text.setFont(this->player_name_font);
		this->player_health_text.setCharacterSize(23);
		this->player_health_text.setFillColor(sf::Color::White);
		this->player_health_text.setPosition(377.f, 394.f);
		this->player_health_text.setString("100 / 100");
		this->player_sprite.setTexture(this->player_texture);
		this->health_sprite.setTexture(this->health_texture);
		this->health_sprite_cover.setTexture(this->health_texture_cover);
		this->health_sprite_cover.setPosition(sf::Vector2f(257.f, 399.f));
		this->health_sprite.setPosition(sf::Vector2f(257.f, 399.f));
		this->player_sprite.setColor(sf::Color(255, 0, 0));
		this->player_sprite.setPosition(sf::Vector2f(316.f, 295.f));
		this->damage_sound.setBuffer(this->damage_buffer);
		this->player_heal.setBuffer(this->player_heal_buffer);
		this->player_attack.setBuffer(this->player_attack_buffer);
		this->miss_attack_sprite.setTexture(this->miss_attack_texture);
		this->miss_attack_sprite.setPosition(sf::Vector2f(278.f, 80.f));
	}
}

void Player::HoverButton(const char button_id)
{
	switch (button_hovered)
	{
		case Button_Type::Fight_Button:
		{
			this->board_instance->HoverButton(Button_Type::Fight_Button);
			this->player_sprite.setPosition(sf::Vector2f(42.f, 445.f));
			break;
		}
		case Button_Type::Act_Button:
		{
			this->board_instance->HoverButton(Button_Type::Act_Button);
			this->player_sprite.setPosition(sf::Vector2f(196.f, 445.f));
			break;
		}
		case Button_Type::Item_Button:
		{
			this->board_instance->HoverButton(Button_Type::Item_Button);
			this->player_sprite.setPosition(sf::Vector2f(356.f, 445.f));
			break;
		}
		case Button_Type::Mercy_Button:
		{
			this->board_instance->HoverButton(Button_Type::Mercy_Button);
			this->player_sprite.setPosition(sf::Vector2f(510.f, 445.f));
			break;
		}
	}
}

void Player::TogglePlayerTurn(const bool toggle, const std::string &board_text, const DWORD board_text_delay)
{
	this->player_turn = toggle;

	if (toggle)
	{
		this->button_pressed = Button_Type::None;
		this->HoverButton(this->button_hovered);
		this->board_instance->ShowBoardText(true);
		this->board_instance->SetBoardText(board_text, board_text_delay);
	}
	else
	{
		this->player_sprite.setPosition(sf::Vector2f(316.f, 295.f));
		this->board_instance->HoverButton(Button_Type::None);
		this->board_instance->ShowBoardText(false);
	}
}

void Player::TakeDamage(const int amount)
{
	if (this->damage_tick < GetTickCount() && this->health > 0 && !this->player_turn)
	{
		this->damage_tick = GetTickCount() + 2000;

		if (this->health - amount < 0)
		{
			this->damage_sound.setBuffer(this->death_buffer);
			this->player_sprite.setTexture(this->player_dead_texture, true);
			this->health = 0;
		}
		else
		{
			this->health -= amount;
		}

		this->damage_sound.play();

		this->health_sprite_cover.setScale(static_cast<float>(this->health) / 100.f, 1.f);
		this->player_health_text.setString(std::to_string(this->health) + " / 100");
	}
}

void Player::Update()
{
	const sf::Vector2f pos = this->player_sprite.getPosition();
	const DWORD current_tick = GetTickCount();
	static DWORD flash_tick = 0, button_hover_move_tick = 0;
	this->is_moving = false;
	
	if (this->health > 0)
	{
		if (this->damage_tick > current_tick)
		{
			if (current_tick > flash_tick)
			{
				flash_tick = current_tick + 200;

				if (this->flash_damage_color == false)
				{
					this->flash_damage_color = true;
					this->player_sprite.setColor(sf::Color(150, 0, 0));
				}
				else
				{
					this->flash_damage_color = false;
					this->player_sprite.setColor(sf::Color(255, 0, 0));
				}
			}
		}
		else if (this->flash_damage_color == true) // Just ensuring it goes back to the default color.
		{
			this->flash_damage_color = false;
			this->player_sprite.setColor(sf::Color(255, 0, 0));
		}

		const sf::FloatRect dimensions = this->board_instance->GetBoardGlobalBounds();
		const sf::Vector2f board_size = this->board_instance->GetBoardSize();

		if (!this->player_turn)
		{
			if (board_size.x > this->board_instance->GetBoardWidth())
			{
				this->board_instance->SetBoardSize(sf::Vector2f(board_size.x - 10.f, 165.f));
			}
			else if (board_size.x < this->board_instance->GetBoardWidth())
			{
				this->board_instance->SetBoardSize(sf::Vector2f(this->board_instance->GetBoardWidth(), 165.f));
			}
			else // Only let the player move after the board is properly resized.
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					if ((pos.x - 3.f) > dimensions.left + 5.5f)
					{
						this->player_sprite.move(-3.f, 0.f);
						this->is_moving = true;
					}
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					if ((pos.x + 3.f) < (dimensions.left + dimensions.width) - 20.5f)
					{
						this->player_sprite.move(3.f, 0.f);
						this->is_moving = true;
					}
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					if ((pos.y - 3.f) > dimensions.top + 5.5f)
					{
						this->player_sprite.move(0.f, -3.f);
						this->is_moving = true;
					}
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					if ((pos.y + 3.f) < (dimensions.top + dimensions.height) - 20.5f)
					{
						this->player_sprite.move(0.f, 3.f);
						this->is_moving = true;
					}
				}
			}
		}
		else
		{
			if (board_size.x < 585.f)
			{
				this->board_instance->SetBoardSize(sf::Vector2f(board_size.x + 10.f, 165.f));
			}
			else if (board_size.x > 585.f)
			{
				this->board_instance->SetBoardSize(sf::Vector2f(585.f, 165.f));
			}

			if (this->button_pressed == Button_Type::None)
			{
				if (current_tick > button_hover_move_tick)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
					{
						if (this->button_hovered == Button_Type::Fight_Button)
						{
							this->button_hovered = Button_Type::Mercy_Button;
						}
						else
						{
							this->button_hovered--;
						}
						this->HoverButton(this->button_hovered);
						button_hover_move_tick = current_tick + 250;
					}

					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
					{
						if (this->button_hovered == Button_Type::Mercy_Button)
						{
							this->button_hovered = Button_Type::Fight_Button;
						}
						else
						{
							this->button_hovered++;
						}
						this->HoverButton(this->button_hovered);
						button_hover_move_tick = current_tick + 250;
					}
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
				{
					const sf::FloatRect player_bounds = this->player_sprite.getGlobalBounds();

					if (player_bounds.intersects(this->board_instance->GetButtonGlobalBounds(Button_Type::Fight_Button)))
					{
						this->button_pressed = Button_Type::Fight_Button;
						this->TogglePlayerTurn(false);
						this->player_attack.play();
						this->use_button_tick = current_tick + 2000;
					}
					else if (player_bounds.intersects(this->board_instance->GetButtonGlobalBounds(Button_Type::Act_Button)))
					{
						this->button_pressed = Button_Type::Act_Button;
						this->player_sprite.setPosition(sf::Vector2f(45.f, 233.f));
						this->board_instance->SetBoardOptionsText("* Check");
						this->board_instance->ShowBoardOptionsText(true);
						this->board_instance->ShowBoardText(false);
						this->use_button_tick = current_tick + 200;
					}
					else if (player_bounds.intersects(this->board_instance->GetButtonGlobalBounds(Button_Type::Item_Button)))
					{
						if (this->heal_items_available)
						{
							this->button_pressed = Button_Type::Item_Button;
							this->player_sprite.setPosition(sf::Vector2f(45.f, 233.f));
							this->board_instance->SetBoardOptionsText("* Pie");
							this->board_instance->ShowBoardOptionsText(true);
							this->board_instance->ShowBoardText(false);
							this->use_button_tick = current_tick + 200;
						}
					}
					else if (player_bounds.intersects(this->board_instance->GetButtonGlobalBounds(Button_Type::Mercy_Button)))
					{
						this->button_pressed = Button_Type::Mercy_Button;
						this->player_sprite.setPosition(sf::Vector2f(45.f, 233.f));
						this->board_instance->SetBoardOptionsText("* Skip Turn");
						this->board_instance->ShowBoardOptionsText(true);
						this->board_instance->ShowBoardText(false);
						this->use_button_tick = current_tick + 200;
					}
				}
			}
			else if (this->button_pressed == Button_Type::Act_Button)
			{
				if (this->use_button_tick < current_tick)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
					{
						if (!this->board_instance->IsBoardTextShown())
						{
							this->board_instance->ShowBoardOptionsText(false);
							this->board_instance->ShowBoardText(true);
							this->board_instance->SetBoardText("* W.D GASTER - ATK ??? DEF ???\n* Like nothing you've seen before.");
							this->HoverButton(this->button_hovered);
							this->use_button_tick = current_tick + 800;
						}
						else
						{
							this->TogglePlayerTurn(false);
							this->use_button_tick = current_tick + 1500;
						}
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && !this->board_instance->IsBoardTextShown())
					{
						this->board_instance->ShowBoardOptionsText(false);
						this->board_instance->ShowBoardText(true);
						this->button_pressed = Button_Type::None;
						this->HoverButton(this->button_hovered);
					}
				}
			}
			else if (this->button_pressed == Button_Type::Item_Button)
			{
				if (this->use_button_tick < current_tick)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
					{
						if (!this->board_instance->IsBoardTextShown())
						{
							this->board_instance->ShowBoardOptionsText(false);
							this->board_instance->ShowBoardText(true);
							this->board_instance->SetBoardText("* You ate the pie.\n* Your HP was maxed out.");
							this->health = 100;
							this->player_heal.play();
							this->player_health_text.setString("100 / 100");
							this->heal_items_available = 0;
							this->health_sprite_cover.setScale(1.f, 1.f);
							this->HoverButton(this->button_hovered);
							this->use_button_tick = current_tick + 800;
						}
						else
						{
							this->TogglePlayerTurn(false);
							this->use_button_tick = current_tick + 1500;
						}
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && !this->board_instance->IsBoardTextShown())
					{
						this->board_instance->ShowBoardOptionsText(false);
						this->board_instance->ShowBoardText(true);
						this->button_pressed = Button_Type::None;
						this->HoverButton(this->button_hovered);
					}
				}
			}
			else if (this->button_pressed == Button_Type::Mercy_Button)
			{
				if (this->use_button_tick < current_tick)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
					{
						this->board_instance->ShowBoardOptionsText(false);
						this->TogglePlayerTurn(false);
						this->use_button_tick = current_tick + 1500;
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
					{
						this->board_instance->ShowBoardOptionsText(false);
						this->board_instance->ShowBoardText(true);
						this->button_pressed = Button_Type::None;
						this->HoverButton(this->button_hovered);
					}
				}
			}
		}

		if (this->use_button_tick > current_tick && this->button_pressed == Button_Type::Fight_Button) // Yeah... this is to "switch" between the attack textures to give some slice effect.
		{
			if (this->use_button_tick - 1900 > current_tick)
			{
				this->miss_attack_sprite.setTexture(this->attack_texture[0], true);
			}
			else if (this->use_button_tick - 1800 > current_tick)
			{
				this->miss_attack_sprite.setTexture(this->attack_texture[1], true);
			}
			else if (this->use_button_tick - 1700 > current_tick)
			{
				this->miss_attack_sprite.setTexture(this->attack_texture[2], true);
			}
			else
			{
				this->miss_attack_sprite.setTexture(this->miss_attack_texture, true);
			}
			this->window_instance->draw(this->miss_attack_sprite);
		}

		this->window_instance->draw(this->health_sprite);
		this->window_instance->draw(this->health_sprite_cover);
		this->window_instance->draw(this->player_name_text);
		this->window_instance->draw(this->player_health_text);
	}
	this->window_instance->draw(this->player_sprite);
}

void Player::Reset()
{
	this->health = 100;
	this->heal_items_available = 1;
	this->player_health_text.setString("100 / 100");
	this->is_moving = false;
	this->damage_tick = 0;
	this->button_pressed = Button_Type::None;
	this->button_hovered = Button_Type::Fight_Button;
	this->player_turn = false;
	this->flash_damage_color = false;
	this->player_sprite.setTexture(this->player_texture, true);
	this->health_sprite_cover.setScale(1.f, 1.f);
	this->player_sprite.setPosition(sf::Vector2f(316.f, 295.f));
	this->damage_sound.setBuffer(this->damage_buffer);
}