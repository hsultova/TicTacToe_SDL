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
	void RenderGrid();
	void ChangeTurn();
	GameState CheckVictory();
	void RestartGame();
	bool CheckBounds(const Position& _position, const Position& _minPosition, const Position& _maxPosition);

	const std::string GetGameState(const GameState& _gameState) const;

private:
	void SwitchPlayer();
	void LoadTextures();
	Player GetPlayer(const Symbol _symbol);
	Position GetGridMinPosition();
	Position GetGridMaxPosition();

private:
	static GameManager* s_instance;

	const int screenWidth = 1080;
	const int screenHeight = 720;

	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;

	Texture* m_xTexture;
	Texture* m_oTexture;
	Texture* m_textTexture;
	Texture* m_restartGameTexture;
	Grid* m_grid;

	Position m_firstHorizontalLinePosition;
	Position m_secondHorizontalLinePosition;
	Position m_firstVerticalLinePosition;
	Position m_secondVerticalLinePosition;
	int m_lineWidth;

	Line m_line;

	Player m_currentPlayer;
	Player m_xPlayer;
	Player m_oPlayer;

	SDL_Color m_xPlayerColor;
	SDL_Color m_oPlayerColor;
	SDL_Color m_mainColor;
	SDL_Color m_gridColor;

	bool m_renderLine = false;
	bool m_endGame = false;
};

