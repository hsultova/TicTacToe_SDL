﻿#include <stdio.h>

#include <assert.h> 

#include "Gameplay\GameManager.h"
#include "Core/Texture.h"
#include "Gameplay\Grid.h"

int main(int argc, char* args[])
{
	GameManager* gameManager = &GameManager();
	GameManager::RegisterInstance(gameManager);

	if (!GameManager::Get()->Initialize())
	{
		printf("Failed to initialize!\n");
		return -1;
	}

	GameManager::Get()->LoadTextures();

	bool quit = false;
	//Event handler
	SDL_Event e;

	Grid* grid = GameManager::Get()->GetGrid();
	assert(grid != nullptr);

	SDL_Renderer* renderer = GameManager::Get()->GetRenderer();
	assert(renderer != nullptr);

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

			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				int x = 0;
				int y = 0;
				SDL_GetMouseState(&x, &y);
				grid->OnMouseClick(x, y);
			}
		}

		//Game Logic
		if (GameManager::Get()->CheckVictory() != GameState::inProgress)
		{
			grid->Clear();
		}

		//Rendering
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		grid->Render();

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Cell cell = grid->grid[i][j];
				if (cell.GetSymbol() == Symbol::o)
				{
					GameManager::Get()->GetOTexture()->Render(cell.GetGlobalPosition().x, cell.GetGlobalPosition().y);
				}
				else if (cell.GetSymbol() == Symbol::x)
				{
					GameManager::Get()->GetXTexture()->Render(cell.GetGlobalPosition().x, cell.GetGlobalPosition().y);
				}
			}
		}

		SDL_RenderPresent(renderer);
	}

	//Free resources and close SDL
	GameManager::Get()->Destroy();
	GameManager::UnregisterInstance(gameManager);

	return 0;
}
