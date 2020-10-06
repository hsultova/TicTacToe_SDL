#include "GameManager.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <assert.h> 

GameManager* GameManager::s_instance = nullptr;

GameManager* GameManager::Get()
{
	return s_instance;
}

void GameManager::RegisterInstance(GameManager* _instance)
{
	assert(s_instance == nullptr);
	s_instance = _instance;
}

void GameManager::UnregisterInstance(GameManager* _instance)
{
	assert(s_instance == _instance);
	s_instance = nullptr;
}

SDL_Window* GameManager::GetWindow() const
{
	return m_window;
}

SDL_Renderer* GameManager::GetRenderer() const
{
	return m_renderer;
}

bool GameManager::Initialize()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning: Linear texture filtering not enabled!");
	}

	//Create window
	m_window = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	if (m_window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Create vsynced renderer for window
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_renderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

	if (IMG_Init(IMG_INIT_JPG) < 0)
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}

	//Initialize class members

	m_oTexture = new Texture();
	m_xTexture = new Texture();
	m_textTexture = new Texture();
	m_restartGameTexture = new Texture();

	m_grid = new Grid();

	m_oPlayerColor = SDL_Color{ 157, 7, 8, 255 };
	m_xPlayerColor = SDL_Color{ 11, 113, 100, 255 };
	m_mainColor = SDL_Color{ 0, 48, 119, 255 };

	m_oPlayer = Player{ "O", Symbol::o, m_oPlayerColor };
	m_xPlayer = Player{ "X", Symbol::x, m_xPlayerColor };
	m_currentPlayer = m_xPlayer;

	return true;
}

void GameManager::Destroy()
{
	delete m_oTexture;
	delete m_xTexture;
	delete m_textTexture;
	delete m_restartGameTexture;
	delete m_grid;

	SDL_DestroyWindow(m_window);
	m_window = nullptr;

	SDL_DestroyRenderer(m_renderer);
	m_renderer = nullptr;

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

const int GameManager::GetScreenWidth() const
{
	return screenWidth;
}

const int GameManager::GetScreenHeight() const
{
	return screenHeight;
}

Player GameManager::GetCurrentPlayer() const
{
	return m_currentPlayer;
}

void GameManager::PlayGame()
{
	LoadTextures();
	m_textTexture->SetFont(TTF_OpenFont("Textures/Lovely_Kids.ttf", 64));
	m_textTexture->LoadFromRenderedText("X Turn", m_xPlayerColor);

	bool quit = false;
	m_renderLine = false;
	m_endGame = false;
	//Event handler
	SDL_Event e;
	SDL_Cursor* cursor;

	assert(m_grid != nullptr);

	SDL_Renderer* renderer = GetRenderer();
	assert(renderer != nullptr);

	int restartGameX = GetScreenWidth() - 300;
	int restartGameY = GetScreenHeight() - 100;

	while (!quit)
	{
		//Input events
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			if (e.type == SDL_MOUSEMOTION)
			{
				int x = 0;
				int y = 0;
				SDL_GetMouseState(&x, &y);

				if ((x < restartGameX + m_restartGameTexture->GetWidth() && x > restartGameX)
					&& (y <restartGameY + m_restartGameTexture->GetHeight() && y >restartGameY))
				{
					cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
					SDL_SetCursor(cursor);
				}
				else
				{
					cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
					SDL_SetCursor(cursor);
				}
			}

			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				int x = 0;
				int y = 0;
				SDL_GetMouseState(&x, &y);

				if (CheckBounds(Position{ x,y }, 
					Position{ restartGameX, restartGameY }, 
					Position{ restartGameX + m_restartGameTexture->GetWidth() ,restartGameY + m_restartGameTexture->GetHeight() }))
				{
					RestartGame();
				}

				if (m_endGame == false && CheckBounds(Position{ x,y }, m_grid->GetMinPosition(), m_grid->GetMaxPosition()))
				{
					m_grid->OnMouseClick(x, y);
				}
			}
		}

		//Game Logic
		GameState gameState = CheckVictory();
		if (gameState != GameState::inProgress)
		{
			m_textTexture->LoadFromRenderedText(GetGameState(gameState), m_mainColor);

			if (gameState != GameState::draw)
			{
				m_renderLine = true;
			}
			m_endGame = true;
		}

		//Rendering
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		m_textTexture->Render(100, GetScreenHeight() - 100);
		m_restartGameTexture->Render(restartGameX, restartGameY);
		m_grid->Render();

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Cell cell = m_grid->grid[i][j];
				if (cell.GetSymbol() == Symbol::o)
				{
					m_oTexture->Render(cell.GetGlobalPosition().x, cell.GetGlobalPosition().y);
				}
				else if (cell.GetSymbol() == Symbol::x)
				{
					m_xTexture->Render(cell.GetGlobalPosition().x, cell.GetGlobalPosition().y);
				}
			}
		}

		if (m_renderLine == true)
		{
			SDL_SetRenderDrawColor(renderer, m_mainColor.r, m_mainColor.g, m_mainColor.b, m_mainColor.a);

			int offset = 25;
			int x = m_line.start.x + offset;
			int y = m_line.start.y + offset;
			while (true)
			{
				SDL_Rect rect = { x, y , 25, 20 };
				SDL_RenderFillRect(renderer, &rect);
				if (x <= m_line.end.x + offset)
				{
					++x;
				}

				if (y <= m_line.end.y + offset)
				{
					++y;
				}

				if (x > m_line.end.x + offset && y > m_line.end.y + offset)
				{
					break;
				}
			}

			//SDL_RenderDrawLine(renderer, line.start.x, line.start.y, line.end.x, line.end.y);
		}

		SDL_RenderPresent(renderer);
	}
}

void GameManager::SwitchPlayer()
{
	if (m_currentPlayer.mark == Symbol::x)
	{
		m_currentPlayer = m_oPlayer;
	}
	else
	{
		m_currentPlayer = m_xPlayer;
	}
}

void GameManager::LoadTextures()
{
	if (m_oTexture == nullptr || m_xTexture == nullptr)
		return;

	m_xTexture->LoadFromFile("Textures/x.JPG");
	m_oTexture->LoadFromFile("Textures/o.JPG");
	m_restartGameTexture->LoadFromFile("Textures/restart_game.JPG");
}

void GameManager::ChangeTurn()
{
	SwitchPlayer();
	m_textTexture->LoadFromRenderedText(m_currentPlayer.name + " Turn", m_currentPlayer.color);
}

Player GameManager::GetPlayer(const Symbol _symbol)
{
	if (_symbol == Symbol::x)
	{
		return m_xPlayer;
	}
	else
	{
		return m_oPlayer;
	}
}

bool GameManager::CheckBounds(const Position& _position, const Position& _minPosition, const Position& _maxPosition)
{
	return (_position.x < _maxPosition.x&& _position.x > _minPosition.x)
		&& (_position.y < _maxPosition.y&& _position.y > _minPosition.y);
}


GameState GameManager::CheckVictory()
{
	bool hasWinner = false;
	Player winner;

	for (int i = 1; i < 3; i++)
	{
		if (m_grid->grid[i][0].GetSymbol() != Symbol::e
			&& m_grid->grid[i][0].GetSymbol() == m_grid->grid[i][1].GetSymbol()
			&& m_grid->grid[i][1].GetSymbol() == m_grid->grid[i][2].GetSymbol())
		{
			winner = GetPlayer(m_grid->grid[i][0].GetSymbol());
			m_line.start = m_grid->grid[i][0].GetGlobalPosition();
			m_line.end = m_grid->grid[i][2].GetGlobalPosition();
			hasWinner = true;
		}
	}

	for (int i = 1; i < 3; i++)
	{
		if (m_grid->grid[0][i].GetSymbol() != Symbol::e
			&& m_grid->grid[0][i].GetSymbol() == m_grid->grid[1][i].GetSymbol()
			&& m_grid->grid[1][i].GetSymbol() == m_grid->grid[2][i].GetSymbol())
		{
			winner = GetPlayer(m_grid->grid[0][i].GetSymbol());
			m_line.start = m_grid->grid[0][i].GetGlobalPosition();
			m_line.end = m_grid->grid[2][i].GetGlobalPosition();
			hasWinner = true;
		}
	}

	if (m_grid->grid[0][0].GetSymbol() != Symbol::e
		&& m_grid->grid[0][0].GetSymbol() == m_grid->grid[1][1].GetSymbol()
		&& m_grid->grid[1][1].GetSymbol() == m_grid->grid[2][2].GetSymbol())
	{
		winner = GetPlayer(m_grid->grid[0][0].GetSymbol());
		m_line.start = m_grid->grid[0][0].GetGlobalPosition();
		m_line.end = m_grid->grid[2][2].GetGlobalPosition();
		hasWinner = true;
	}

	if (m_grid->grid[0][2].GetSymbol() != Symbol::e
		&& m_grid->grid[0][2].GetSymbol() == m_grid->grid[1][1].GetSymbol()
		&& m_grid->grid[1][1].GetSymbol() == m_grid->grid[2][0].GetSymbol())
	{
		winner = GetPlayer(m_grid->grid[0][2].GetSymbol());
		m_line.start = m_grid->grid[0][2].GetGlobalPosition();
		m_line.end = m_grid->grid[2][0].GetGlobalPosition();
		hasWinner = true;
	}

	if (hasWinner)
	{
		if (winner.mark == Symbol::x)
		{
			return GameState::xWon;
		}
		else
		{
			return GameState::oWon;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (m_grid->grid[i][j].GetSymbol() == Symbol::e)
			{
				return GameState::inProgress;
			}
		}
	}

	return GameState::draw;
}

void GameManager::RestartGame()
{
	m_endGame = false;
	m_renderLine = false;
	m_grid->Clear();
	m_currentPlayer = m_xPlayer;
	GameManager::Get()->m_textTexture->LoadFromRenderedText(GameManager::Get()->GetCurrentPlayer().name + " Turn", GameManager::Get()->GetCurrentPlayer().color);
}

const std::string GameManager::GetGameState(const GameState& _gameState) const
{
	switch (_gameState)
	{
	case GameState::draw:
		return "Draw";
	case GameState::inProgress:
		return "In Progress";
	case GameState::oWon:
		return "O Won";
	case GameState::xWon:
		return "X Won";
	}
	return std::string();
}
