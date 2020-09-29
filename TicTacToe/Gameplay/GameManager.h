#pragma once

#include <SDL.h>

#include "Core/Texture.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

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

	void LoadTextures();

private:
	static GameManager* s_instance;

	const int screenWidth = 720;
	const int screenHeight = 480;

	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;

	Texture* m_xTexture;
	Texture* m_oTexture;
};

