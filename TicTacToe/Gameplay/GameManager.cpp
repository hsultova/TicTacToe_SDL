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

Texture* GameManager::GetXTexture() const
{
	return m_xTexture;
}

Texture* GameManager::GetOTexture() const
{
	return m_oTexture;
}

Texture* GameManager::GetTextTexture() const
{
	return m_textTexture;
}

Grid* GameManager::GetGrid() const
{
	return m_grid;
}

Line GameManager::GetLine() const
{
	return m_line;
}

Player GameManager::GetCurrentPlayer() const
{
	return m_currentPlayer;
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
}

void GameManager::ChangeTurn()
{
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

const SDL_Color GameManager::GetXPlayerColor() const
{
	return m_xPlayerColor;
}

const SDL_Color GameManager::GetOPlayerColor() const
{
	return m_oPlayerColor;
}

const SDL_Color GameManager::GetMainColor() const
{
	return m_mainColor;
}
