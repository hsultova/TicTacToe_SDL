#include <stdio.h>

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
				GameManager::Get()->GetGrid()->OnMouseClick(x, y);
			}
		}

		//Game Logic
		if (GameManager::Get()->HasVictory())
		{

		}

		//Rendering
		assert(GameManager::Get()->GetRenderer() != nullptr);
		SDL_SetRenderDrawColor(GameManager::Get()->GetRenderer(), 255, 255, 255, 255);
		SDL_RenderClear(GameManager::Get()->GetRenderer());

		GameManager::Get()->GetGrid()->Render();

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Cell cell = GameManager::Get()->GetGrid()->grid[i][j];
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

		SDL_RenderPresent(GameManager::Get()->GetRenderer());
	}

	//Free resources and close SDL
	GameManager::Get()->Destroy();
	GameManager::UnregisterInstance(gameManager);

	return 0;
}
