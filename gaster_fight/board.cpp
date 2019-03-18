#include "board.h"

Board::Board(sf::RenderWindow *window_instance) : board_width(165.f), board_text_position(0), board_text_tick(0)
{
	FileManager::LoadFromFile(this->fight_button_texture, "bin\\sprites\\fightbt.png");
	FileManager::LoadFromFile(this->fight_button_hover_texture, "bin\\sprites\\fightbt_hover.png");
	FileManager::LoadFromFile(this->act_button_texture, "bin\\sprites\\actbt.png");
	FileManager::LoadFromFile(this->act_button_hover_texture, "bin\\sprites\\actbt_hover.png");
	FileManager::LoadFromFile(this->item_button_texture, "bin\\sprites\\itembt.png");
	FileManager::LoadFromFile(this->item_button_hover_texture, "bin\\sprites\\itembt_hover.png");
	FileManager::LoadFromFile(this->mercy_button_texture, "bin\\sprites\\mercybt.png");
	FileManager::LoadFromFile(this->mercy_button_hover_texture, "bin\\sprites\\mercybt_hover.png");
	FileManager::LoadFromFile(this->hp_texture, "bin\\sprites\\hp.png");
	FileManager::LoadFromFile(this->board_text_font, "bin\\fonts\\determination_mono.ttf");
	FileManager::LoadFromFile(this->text_update_sound_buffer, "bin\\audio\\border_text.wav");

	if (!FileManager::IsAnyFileMissing())
	{
		this->window_instance = window_instance;
		this->hp_sprite.setTexture(this->hp_texture);
		this->hp_sprite.setPosition(sf::Vector2f(226.f, 404.f));
		this->board_rectangle.setPosition(sf::Vector2f(325.f, 302.f));
		this->board_rectangle.setSize(sf::Vector2f(100.f, 165.f));
		this->board_rectangle.setOrigin(sf::Vector2f(this->board_rectangle.getLocalBounds().width / 2.f, this->board_rectangle.getLocalBounds().height / 2.f));
		this->board_rectangle.setOutlineColor(sf::Color::White);
		this->board_rectangle.setFillColor(sf::Color(255, 255, 255, 0));
		this->board_rectangle.setOutlineThickness(5);
		this->fight_button_sprite.setTexture(this->fight_button_texture);
		this->act_button_sprite.setTexture(this->act_button_texture);
		this->item_button_sprite.setTexture(this->item_button_texture);
		this->mercy_button_sprite.setTexture(this->mercy_button_texture);
		this->fight_button_sprite.setPosition(sf::Vector2f(34.f, 431.f));
		this->act_button_sprite.setPosition(sf::Vector2f(187.f, 431.f));
		this->item_button_sprite.setPosition(sf::Vector2f(347.f, 431.f));
		this->mercy_button_sprite.setPosition(sf::Vector2f(502.f, 431.f));
		this->board_text.setFont(this->board_text_font);
		this->board_text.setCharacterSize(30);
		this->board_text.setFillColor(sf::Color::White);
		this->board_text.setPosition(50.f, 220.f);
		this->board_options_text.setFont(this->board_text_font);
		this->board_options_text.setCharacterSize(30);
		this->board_options_text.setFillColor(sf::Color::White);
		this->board_options_text.setPosition(70.f, 220.f);
		this->show_board_text = false;
		this->show_board_options_text = false;
		this->text_update_sound.setBuffer(this->text_update_sound_buffer);
	}
}

void Board::SetBoardText(const std::string &text, const DWORD delay)
{
	this->board_text_tick = GetTickCount() + delay;
	this->board_text_position = 0;
	this->board_text.setString("");
	this->board_text_buffer = text;
}

sf::FloatRect Board::GetButtonGlobalBounds(const char button_type) const
{
	switch (button_type)
	{
		case Button_Type::Fight_Button:
		{
			return this->fight_button_sprite.getGlobalBounds();
		}
		case Button_Type::Act_Button:
		{
			return this->act_button_sprite.getGlobalBounds();
		}
		case Button_Type::Item_Button:
		{
			return this->item_button_sprite.getGlobalBounds();
		}
		case Button_Type::Mercy_Button:
		{
			return this->mercy_button_sprite.getGlobalBounds();
		}
	}
}

void Board::HoverButton(const char button_type)
{
	switch (button_type)
	{
		case Button_Type::Fight_Button:
		{
			this->fight_button_sprite.setTexture(this->fight_button_hover_texture);
			this->act_button_sprite.setTexture(this->act_button_texture);
			this->item_button_sprite.setTexture(this->item_button_texture);
			this->mercy_button_sprite.setTexture(this->mercy_button_texture);
			break;
		}
		case Button_Type::Act_Button:
		{
			this->fight_button_sprite.setTexture(this->fight_button_texture);
			this->act_button_sprite.setTexture(this->act_button_hover_texture);
			this->item_button_sprite.setTexture(this->item_button_texture);
			this->mercy_button_sprite.setTexture(this->mercy_button_texture);
			break;
		}
		case Button_Type::Item_Button:
		{
			this->fight_button_sprite.setTexture(this->fight_button_texture);
			this->act_button_sprite.setTexture(this->act_button_texture);
			this->item_button_sprite.setTexture(this->item_button_hover_texture);
			this->mercy_button_sprite.setTexture(this->mercy_button_texture);
			break;
		}
		case Button_Type::Mercy_Button:
		{
			this->fight_button_sprite.setTexture(this->fight_button_texture);
			this->act_button_sprite.setTexture(this->act_button_texture);
			this->item_button_sprite.setTexture(this->item_button_texture);
			this->mercy_button_sprite.setTexture(this->mercy_button_hover_texture);
			break;
		}
		default:
		{
			this->fight_button_sprite.setTexture(this->fight_button_texture);
			this->act_button_sprite.setTexture(this->act_button_texture);
			this->item_button_sprite.setTexture(this->item_button_texture);
			this->mercy_button_sprite.setTexture(this->mercy_button_texture);
		}
	}
}

void Board::Update()
{
	const DWORD current_tick = GetTickCount();

	this->window_instance->draw(this->board_rectangle);
	this->window_instance->draw(this->hp_sprite);
	this->window_instance->draw(this->fight_button_sprite);
	this->window_instance->draw(this->act_button_sprite);
	this->window_instance->draw(this->item_button_sprite);
	this->window_instance->draw(this->mercy_button_sprite);

	if (this->show_board_text)
	{
		if (this->board_text_position != this->board_text_buffer.size() && (current_tick > this->board_text_tick))
		{
			this->board_text_tick = current_tick + 50;
			this->board_text.setString(this->board_text.getString() + this->board_text_buffer[this->board_text_position]);
			this->board_text_position++;
			this->text_update_sound.play();
		}

		this->window_instance->draw(this->board_text);
	}

	if (this->show_board_options_text)
	{
		this->window_instance->draw(this->board_options_text);
	}
}