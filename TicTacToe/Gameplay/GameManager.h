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

	Texture* GetXTexture() const;
	Texture* GetOTexture() const;
	Texture* GetTextTexture() const;

	Grid* GetGrid() const;

	Player GetCurrentPlayer() const;

	void SwitchPlayer();
	void LoadTextures();
	void ChangeTurn();
	Player GetPlayer(const Symbol _symbol);
	GameState CheckVictory();

	const std::string GetGameState(const GameState& _gameState) const;

	const SDL_Color GetXPlayerColor() const;
	const SDL_Color GetOPlayerColor() const;
	const SDL_Color GetMainColor() const;

private:
	static GameManager* s_instance;

	const int screenWidth = 720;
	const int screenHeight = 480;

	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;

	Texture* m_xTexture;
	Texture* m_oTexture;
	Texture* m_textTexture;
	Grid* m_grid;

	Player m_currentPlayer;
	Player m_xPlayer;
	Player m_oPlayer;

	SDL_Color m_xPlayerColor;
	SDL_Color m_oPlayerColor;
	SDL_Color m_mainColor;
};

