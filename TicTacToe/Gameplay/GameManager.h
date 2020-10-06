#pragma once

#include <SDL.h>

#include <string>

#include "..\Core\Texture.h"
#include "Grid.h"

struct Player
{
	std::string name;
	Symbol mark;
	SDL_Color color;
};

struct Line
{
	Position start;
	Position end;
};

enum class GameState
{
	inProgress,
	draw,
	xWon,
	oWon
};

class GameManager
{
public:
	static GameManager* Get();

	static void RegisterInstance(GameManager* _instance);
	static void UnregisterInstance(GameManager* _instance);

	SDL_Window* GetWindow() const;
	SDL_Renderer* GetRenderer() const;

	bool Initialize();
	void Destroy();

	const int GetScreenWidth() const;
	const int GetScreenHeight() const;

	Player GetCurrentPlayer() const;

	void PlayGame();
	void ChangeTurn();
	GameState CheckVictory();

	const std::string GetGameState(const GameState& _gameState) const;

private:
	void SwitchPlayer();
	void LoadTextures();
	Player GetPlayer(const Symbol _symbol);

private:
	static GameManager* s_instance;

	const int screenWidth = 720;
	const int screenHeight = 480;

	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;

	Texture* m_xTexture;
	Texture* m_oTexture;
	Texture* m_textTexture;
	Texture* m_restartGameTexture;
	Grid* m_grid;

	Line m_line;

	Player m_currentPlayer;
	Player m_xPlayer;
	Player m_oPlayer;

	SDL_Color m_xPlayerColor;
	SDL_Color m_oPlayerColor;
	SDL_Color m_mainColor;
};

