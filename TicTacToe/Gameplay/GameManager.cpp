#include "GameManager.h"

#include <SDL_image.h>
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

GameManager::GameManager()
{
	m_oTexture = new Texture();
	m_xTexture = new Texture();

	m_grid = new Grid();
	
	m_oPlayer = Player{ "o", Symbol::o };
	m_xPlayer = Player{ "x", Symbol::x };
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

	return true;
}

void GameManager::Destroy()
{
	delete m_oTexture;
	delete m_xTexture;
	delete m_grid;

	SDL_DestroyWindow(m_window);
	m_window = nullptr;

	SDL_DestroyRenderer(m_renderer);
	m_renderer = nullptr;

	IMG_Quit();
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

Grid* GameManager::GetGrid() const
{
	return m_grid;
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

bool GameManager::HasVictory()
{
	return false;
}
