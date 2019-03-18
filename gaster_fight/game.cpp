#include "game.h"

Game::Game() : window(sf::VideoMode(642, 481), "Gaster Fight", sf::Style::Titlebar | sf::Style::Close), current_turn(0), turn_tick(0)
{
	this->board = new Board(&this->window);
	this->player = new Player(&this->window, this->board);
	this->gaster = new Gaster(this->player, &this->window);

	FileManager::LoadFromFile(this->game_icon, "bin\\sprites\\game.png");
	FileManager::LoadFromFile(this->game_music, "bin\\audio\\megalovania.ogg");
	FileManager::LoadFromFile(this->game_over_texture, "bin\\sprites\\game_over.png");
	FileManager::LoadFromFile(this->credits_text_font, "bin\\fonts\\determination_mono.ttf");

	if (!FileManager::IsAnyFileMissing())
	{
		this->window.setFramerateLimit(60);
		this->game_music.setLoop(true);
		this->game_music.play();
		this->game_over_sprite.setTexture(this->game_over_texture);
		this->game_over_sprite.setPosition(sf::Vector2f(121.f, 30.f));
		this->window.setIcon(20, 19, this->game_icon.getPixelsPtr());
		this->credits_text.setFont(this->credits_text_font);
		this->credits_text.setCharacterSize(20);
		this->credits_text.setFillColor(sf::Color::White);
		this->credits_text.setPosition(50.f, 220.f);
		this->credits_text.setString("Credits:\n* Toby Fox for Undertale.\n* Haley Wakamatsu for Determination Mono font.\n* hotspicytaco for testing.\n* And you for playing this badly done fan-game.");
	}
}

void Game::Update()
{
	constexpr int last_turn = 27;

	while (this->window.isOpen())
	{
		sf::Event event;
		const DWORD current_tick = GetTickCount();

		while (this->window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				this->window.close();
			}
		}

		this->window.clear();

		if (this->player->GetPlayerHealth() > 0 && this->current_turn != last_turn) // If current_turn equals last_turn, the game finishes.
		{
			this->board->Update();

			if (!this->player->IsPlayerInTurn())
			{
				if (this->turn_tick < current_tick && this->player->GetButtonUseTick() < current_tick)
				{
					switch (this->current_turn)
					{
						case 0:
						{
							this->current_turn = 1;
							this->turn_tick = current_tick + 3100;
							this->gaster->SetText("So we finally meet...", 3100);
							break;
						}
						case 1:
						{
							this->current_turn = 2;
							this->turn_tick = current_tick + 6000;
							this->gaster->SetText("You think you are above\nconsequences, don't you?", 6000);
							break;
						}
						case 2:
						{
							this->current_turn = 3;
							this->turn_tick = current_tick + 5000;
							this->gaster->SetText("Well...\nLet's get this over with.", 5000);
							break;
						}
						case 3:
						{
							this->current_turn = 4;
							this->turn_tick = current_tick + 2750;

							this->gaster->AddBlaster(sf::Vector2f(256.f, 90.f), Blaster_Direction::Direction_Down, 0, Blaster_Type::Default_Blaster, 1000);
							this->gaster->AddBlaster(sf::Vector2f(530.f, 235.f), Blaster_Direction::Direction_Left, 0, Blaster_Type::Default_Blaster, 1000);
							this->gaster->AddBlaster(sf::Vector2f(181.f, 90.f), Blaster_Direction::Direction_Down, 1200, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(530.f, 301.f), Blaster_Direction::Direction_Left, 1200, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(211.f, 90.f), Blaster_Direction::Direction_Down, 1350, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(530.f, 271.f), Blaster_Direction::Direction_Left, 1350, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(241.f, 90.f), Blaster_Direction::Direction_Down, 1500, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(530.f, 241.f), Blaster_Direction::Direction_Left, 1500, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(274.f, 90.f), Blaster_Direction::Direction_Down, 1650, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(530.f, 208.f), Blaster_Direction::Direction_Left, 1650, Blaster_Type::Default_Blaster, 120);
							break;
						}
						case 4:
						{
							this->current_turn = 5;
							this->turn_tick = current_tick + 720;

							this->gaster->AddBlaster(sf::Vector2f(player->GetPlayerPosition().x - 60.f, 90.f), Blaster_Direction::Direction_Down, 0, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(530.f, player->GetPlayerPosition().y - 60.f), Blaster_Direction::Direction_Left, 0, Blaster_Type::Default_Blaster, 120);
							break;
						}
						case 5:
						{
							this->current_turn = 6;
							this->turn_tick = current_tick + 720;

							this->gaster->AddBlaster(sf::Vector2f(player->GetPlayerPosition().x - 60.f, 90.f), Blaster_Direction::Direction_Down, 0, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(530.f, player->GetPlayerPosition().y - 60.f), Blaster_Direction::Direction_Left, 0, Blaster_Type::Default_Blaster, 120);
							break;
						}
						case 6:
						{
							this->current_turn = 7;
							this->turn_tick = current_tick + 1200;

							this->gaster->AddBlaster(sf::Vector2f(player->GetPlayerPosition().x - 60.f, 90.f), Blaster_Direction::Direction_Down, 0, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(530.f, player->GetPlayerPosition().y - 60.f), Blaster_Direction::Direction_Left, 0, Blaster_Type::Default_Blaster, 120);
							break;
						}
						case 7:
						{
							this->player->TogglePlayerTurn(true, "* You are gonna have a bad time.", 700);
							this->board->SetBoardWidth(400.f);
							this->current_turn = 8;
							break;
						}
						case 8:
						{
							this->turn_tick = current_tick + 3100;
							this->gaster->AddBlaster(sf::Vector2f(70.f, 90.f), Blaster_Direction::Direction_Down);
							this->gaster->AddBlaster(sf::Vector2f(106.f, 90.f), Blaster_Direction::Direction_Down, 300);
							this->gaster->AddBlaster(sf::Vector2f(154.f, 90.f), Blaster_Direction::Direction_Down, 600);
							this->gaster->AddBlaster(sf::Vector2f(199.f, 90.f), Blaster_Direction::Direction_Down, 900);
							this->gaster->AddBlaster(sf::Vector2f(226.f, 90.f), Blaster_Direction::Direction_Down, 1200);
							this->gaster->AddBlaster(sf::Vector2f(286.f, 90.f), Blaster_Direction::Direction_Down, 1500);
							this->gaster->AddBlaster(sf::Vector2f(346.f, 90.f), Blaster_Direction::Direction_Down, 1800);
							this->gaster->AddBlaster(sf::Vector2f(5.f, 340.f), Blaster_Direction::Direction_Right, 2100);
							this->gaster->AddBlaster(sf::Vector2f(638.f, 280.f), Blaster_Direction::Direction_Left, 2100);
							this->current_turn = 9;
							break;
						}
						case 9:
						{
							this->current_turn = 10;
							this->gaster->ToggleGasterSurprised(true);
							this->turn_tick = current_tick + 10000;
							this->gaster->SetText("Let's see how you\nhandle this...", 4500);

							this->gaster->AddBlaster(sf::Vector2f(70.f, 90.f), Blaster_Direction::Direction_Down, 4600, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(106.f, 90.f), Blaster_Direction::Direction_Down, 4600, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(154.f, 90.f), Blaster_Direction::Direction_Down, 4600, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(199.f, 90.f), Blaster_Direction::Direction_Down, 4600, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(250.f, 90.f), Blaster_Direction::Direction_Down, 4600, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(298.f, 90.f), Blaster_Direction::Direction_Down, 4600, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(337.f, 90.f), Blaster_Direction::Direction_Down, 4600, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(373.f, 90.f), Blaster_Direction::Direction_Down, 4600, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(409.f, 90.f), Blaster_Direction::Direction_Down, 4600, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(442.f, 90.f), Blaster_Direction::Direction_Down, 4600, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(70.f, 90.f), Blaster_Direction::Direction_Down, 5800, Blaster_Type::Orange_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(106.f, 90.f), Blaster_Direction::Direction_Down, 5800, Blaster_Type::Orange_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(154.f, 90.f), Blaster_Direction::Direction_Down, 5800, Blaster_Type::Orange_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(199.f, 90.f), Blaster_Direction::Direction_Down, 5800, Blaster_Type::Orange_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(250.f, 90.f), Blaster_Direction::Direction_Down, 5800, Blaster_Type::Orange_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(298.f, 90.f), Blaster_Direction::Direction_Down, 5800, Blaster_Type::Orange_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(337.f, 90.f), Blaster_Direction::Direction_Down, 5800, Blaster_Type::Orange_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(373.f, 90.f), Blaster_Direction::Direction_Down, 5800, Blaster_Type::Orange_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(409.f, 90.f), Blaster_Direction::Direction_Down, 5800, Blaster_Type::Orange_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(442.f, 90.f), Blaster_Direction::Direction_Down, 5800, Blaster_Type::Orange_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(70.f, 90.f), Blaster_Direction::Direction_Down, 7000, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(106.f, 90.f), Blaster_Direction::Direction_Down, 7000, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(154.f, 90.f), Blaster_Direction::Direction_Down, 7000, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(199.f, 90.f), Blaster_Direction::Direction_Down, 7000, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(250.f, 90.f), Blaster_Direction::Direction_Down, 7000, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(298.f, 90.f), Blaster_Direction::Direction_Down, 7000, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(337.f, 90.f), Blaster_Direction::Direction_Down, 7000, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(373.f, 90.f), Blaster_Direction::Direction_Down, 7000, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(409.f, 90.f), Blaster_Direction::Direction_Down, 7000, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(442.f, 90.f), Blaster_Direction::Direction_Down, 7000, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(70.f, 90.f), Blaster_Direction::Direction_Down, 8200, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(106.f, 90.f), Blaster_Direction::Direction_Down, 8200, Blaster_Type::Default_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(154.f, 90.f), Blaster_Direction::Direction_Down, 8200, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(199.f, 90.f), Blaster_Direction::Direction_Down, 8200, Blaster_Type::Default_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(250.f, 90.f), Blaster_Direction::Direction_Down, 8200, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(298.f, 90.f), Blaster_Direction::Direction_Down, 8200, Blaster_Type::Default_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(337.f, 90.f), Blaster_Direction::Direction_Down, 8200, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(373.f, 90.f), Blaster_Direction::Direction_Down, 8200, Blaster_Type::Default_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(409.f, 90.f), Blaster_Direction::Direction_Down, 8200, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(442.f, 90.f), Blaster_Direction::Direction_Down, 8200, Blaster_Type::Default_Blaster, 500);
							break;
						}
						case 10:
						{
							this->gaster->ToggleGasterSurprised(false);
							this->player->TogglePlayerTurn(true, "* You feel like you can't win.", 700);
							this->current_turn = 11;
							break;
						}
						case 11:
						{
							this->turn_tick = current_tick + 720;
							this->current_turn = 12;

							this->gaster->AddBlaster(sf::Vector2f(player->GetPlayerPosition().x - 60.f, 90.f), Blaster_Direction::Direction_Down, 0, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(638.f, player->GetPlayerPosition().y - 60.f), Blaster_Direction::Direction_Left, 0, Blaster_Type::Default_Blaster, 120);
							break;
						}
						case 12:
						{
							this->turn_tick = current_tick + 720;
							this->current_turn = 13;

							this->gaster->AddBlaster(sf::Vector2f(player->GetPlayerPosition().x - 60.f, 90.f), Blaster_Direction::Direction_Down, 0, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(638.f, player->GetPlayerPosition().y - 60.f), Blaster_Direction::Direction_Left, 0, Blaster_Type::Default_Blaster, 120);
							break;
						}
						case 13:
						{
							this->turn_tick = current_tick + 720;
							this->current_turn = 14;

							this->gaster->AddBlaster(sf::Vector2f(player->GetPlayerPosition().x - 60.f, 90.f), Blaster_Direction::Direction_Down, 0, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(638.f, player->GetPlayerPosition().y - 60.f), Blaster_Direction::Direction_Left, 0, Blaster_Type::Default_Blaster, 120);
							break;
						}
						case 14:
						{
							this->turn_tick = current_tick + 720;
							this->current_turn = 15;

							this->gaster->AddBlaster(sf::Vector2f(player->GetPlayerPosition().x - 60.f, 90.f), Blaster_Direction::Direction_Down, 0, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(638.f, player->GetPlayerPosition().y - 60.f), Blaster_Direction::Direction_Left, 0, Blaster_Type::Default_Blaster, 120);
							break;
						}
						case 15:
						{
							this->turn_tick = current_tick + 720;
							this->current_turn = 16;

							this->gaster->AddBlaster(sf::Vector2f(player->GetPlayerPosition().x - 60.f, 90.f), Blaster_Direction::Direction_Down, 0, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(638.f, player->GetPlayerPosition().y - 60.f), Blaster_Direction::Direction_Left, 0, Blaster_Type::Default_Blaster, 120);
							break;
						}
						case 16:
						{
							this->turn_tick = current_tick + 2510;
							this->current_turn = 17;

							this->gaster->AddBlaster(sf::Vector2f(player->GetPlayerPosition().x - 60.f, 90.f), Blaster_Direction::Direction_Down, 0, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(638.f, player->GetPlayerPosition().y - 60.f), Blaster_Direction::Direction_Left, 0, Blaster_Type::Default_Blaster, 120);

							this->gaster->AddBlaster(sf::Vector2f(70.f, 90.f), Blaster_Direction::Direction_Down, 800, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(106.f, 90.f), Blaster_Direction::Direction_Down, 800, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(154.f, 90.f), Blaster_Direction::Direction_Down, 800, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(199.f, 90.f), Blaster_Direction::Direction_Down, 800, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(250.f, 90.f), Blaster_Direction::Direction_Down, 800, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(298.f, 90.f), Blaster_Direction::Direction_Down, 800, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(337.f, 90.f), Blaster_Direction::Direction_Down, 800, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(373.f, 90.f), Blaster_Direction::Direction_Down, 800, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(409.f, 90.f), Blaster_Direction::Direction_Down, 800, Blaster_Type::Blue_Blaster, 500);
							this->gaster->AddBlaster(sf::Vector2f(442.f, 90.f), Blaster_Direction::Direction_Down, 800, Blaster_Type::Blue_Blaster, 500);
							break;
						}
						case 17:
						{
							this->player->TogglePlayerTurn(true, "* He is preparing something...", 700);
							this->board->SetBoardWidth(165.f);
							this->current_turn = 18;
							break;
						}
						case 18:
						{
							this->turn_tick = current_tick + 2100;
							this->current_turn = 19;

							this->gaster->SetText("Let's end this.", 2000);
							this->gaster->ToggleGasterSurprised(true);
							break;
						}
						case 19:
						{
							this->gaster->ToggleGasterSurprised(false);
							this->turn_tick = current_tick + 720;
							this->current_turn = 20;

							this->gaster->AddBlaster(sf::Vector2f(player->GetPlayerPosition().x - 60.f, 90.f), Blaster_Direction::Direction_Down, 0, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(638.f, player->GetPlayerPosition().y - 60.f), Blaster_Direction::Direction_Left, 0, Blaster_Type::Default_Blaster, 120);
							break;
						}
						case 20:
						{
							this->turn_tick = current_tick + 720;
							this->current_turn = 21;

							this->gaster->AddBlaster(sf::Vector2f(player->GetPlayerPosition().x - 60.f, 90.f), Blaster_Direction::Direction_Down, 0, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(638.f, player->GetPlayerPosition().y - 60.f), Blaster_Direction::Direction_Left, 0, Blaster_Type::Default_Blaster, 120);
							break;
						}
						case 21:
						{
							this->turn_tick = current_tick + 720;
							this->current_turn = 22;

							this->gaster->AddBlaster(sf::Vector2f(player->GetPlayerPosition().x - 60.f, 90.f), Blaster_Direction::Direction_Down, 0, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(638.f, player->GetPlayerPosition().y - 60.f), Blaster_Direction::Direction_Left, 0, Blaster_Type::Default_Blaster, 120);
							break;
						}
						case 22:
						{
							this->turn_tick = current_tick + 720;
							this->current_turn = 23;

							this->gaster->AddBlaster(sf::Vector2f(player->GetPlayerPosition().x - 60.f, 90.f), Blaster_Direction::Direction_Down, 0, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(638.f, player->GetPlayerPosition().y - 60.f), Blaster_Direction::Direction_Left, 0, Blaster_Type::Default_Blaster, 120);
							break;
						}
						case 23:
						{
							this->turn_tick = current_tick + 3200;
							this->current_turn = 24;
							this->gaster->AddBlaster(sf::Vector2f(181.f, 90.f), Blaster_Direction::Direction_Down, 200, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(530.f, 301.f), Blaster_Direction::Direction_Left, 200, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(211.f, 90.f), Blaster_Direction::Direction_Down, 350, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(530.f, 271.f), Blaster_Direction::Direction_Left, 350, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(241.f, 90.f), Blaster_Direction::Direction_Down, 500, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(530.f, 241.f), Blaster_Direction::Direction_Left, 500, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(274.f, 90.f), Blaster_Direction::Direction_Down, 650, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(530.f, 208.f), Blaster_Direction::Direction_Left, 650, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(325.f, 90.f), Blaster_Direction::Direction_Down, 1000, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(530.f, 157.f), Blaster_Direction::Direction_Left, 1000, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(298.f, 90.f), Blaster_Direction::Direction_Down, 1150, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(530.f, 187.f), Blaster_Direction::Direction_Left, 1150, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(265.f, 90.f), Blaster_Direction::Direction_Down, 1300, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(530.f, 223.f), Blaster_Direction::Direction_Left, 1300, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(238.f, 90.f), Blaster_Direction::Direction_Down, 1550, Blaster_Type::Default_Blaster, 120);
							this->gaster->AddBlaster(sf::Vector2f(530.f, 253.f), Blaster_Direction::Direction_Left, 1550, Blaster_Type::Default_Blaster, 120);
							break;
						}
						case 24:
						{
							this->turn_tick = current_tick + 4600;
							this->current_turn = 25;
							this->gaster->SetText("That is it...\nNo more playing around.", 4500);
							this->game_music.pause();
							break;
						}
						case 25:
						{
							this->turn_tick = current_tick + 5100;
							this->current_turn = 26;
							this->gaster->ToggleGasterSurprised(true);
							this->gaster->SetText("Since when were you the\none in control?", 5100);
							this->gaster->AddBlaster(sf::Vector2f(181.f, 90.f), Blaster_Direction::Direction_Down, 200, Blaster_Type::Default_Blaster, 120, 100000);
							this->gaster->AddBlaster(sf::Vector2f(217.f, 90.f), Blaster_Direction::Direction_Down, 200, Blaster_Type::Default_Blaster, 120, 100000);
							this->gaster->AddBlaster(sf::Vector2f(250.f, 90.f), Blaster_Direction::Direction_Down, 200, Blaster_Type::Default_Blaster, 120, 100000);
							this->gaster->AddBlaster(sf::Vector2f(271.f, 90.f), Blaster_Direction::Direction_Down, 200, Blaster_Type::Default_Blaster, 120, 100000);
							this->gaster->AddBlaster(sf::Vector2f(301.f, 90.f), Blaster_Direction::Direction_Down, 200, Blaster_Type::Default_Blaster, 120, 100000);
							this->gaster->AddBlaster(sf::Vector2f(325.f, 90.f), Blaster_Direction::Direction_Down, 200, Blaster_Type::Default_Blaster, 120, 100000);
							break;
						}
						case 26:
						{
							this->current_turn = 27; // This turn means the game finished.
							break;
						}
					}
				}
			}
			this->gaster->Update();
		}
		else
		{
			if (this->current_turn != last_turn) // If we got here and the current turn isn't equal to last_turn, the player died.
			{
				if (this->game_music.getStatus() != sf::Music::Status::Stopped)
				{
					this->game_music.stop();
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
				{
					this->board->SetBoardWidth(165.f);
					this->player->Reset();
					this->gaster->Reset();
					this->game_music.play();
					this->current_turn = 2;
					this->turn_tick = 0;
				}
				this->window.draw(this->game_over_sprite);
			}
			else // If we got here, we reached the last turn and the game finished.
			{
				if (this->game_music.getStatus() == sf::Music::Status::Paused)
				{
					this->game_music.play();
					this->gaster->Reset();
				}
				this->window.draw(this->credits_text);
			}
		}

		if (this->current_turn != last_turn) // Player updates should stop on the last turn (in this case, the game finished).
		{
			this->player->Update();
		}

		this->window.display();
	}
}