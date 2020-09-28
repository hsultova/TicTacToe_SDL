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
}

GameManager::~GameManager()
{
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
	SDL_DestroyWindow(m_window);
	m_window = nullptr;

	SDL_DestroyRenderer(m_renderer);
	m_renderer = nullptr;

	IMG_Quit();
	SDL_Quit();
}

void GameManager::DrawGrid()
{
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 139, 255);
	int offset = 100;
	int width = screenWidth - 2 * offset;
	int height = 5;

	//Draw grid using rectangles. We make the line bolder this way.
	SDL_Rect firstHorizontalLine = { offset, screenHeight / 4, width, height };
	SDL_RenderFillRect(m_renderer, &firstHorizontalLine);

	SDL_Rect secondHorizontalLine = { offset, screenHeight / 3 + offset, width, height };
	SDL_RenderFillRect(m_renderer, &secondHorizontalLine);

	SDL_Rect firstVerticallLine = { static_cast<int>(2.5 * offset), screenHeight / offset, height, width - static_cast<int>(1.5 * offset) };
	SDL_RenderFillRect(m_renderer, &firstVerticallLine);

	SDL_Rect secondVerticalLine = { static_cast<int>(4.5 * offset), screenHeight / offset, height , width - static_cast<int>(1.5 * offset) };
	SDL_RenderFillRect(m_renderer, &secondVerticalLine);
}
