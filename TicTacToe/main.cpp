#include <stdio.h>

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

	int x = 0;
	int y = 0;

	Grid grid;



	while (!quit)
	{
		//Input events
		//Game Logic
		//Rendering



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
				SDL_GetMouseState(&x, &y);
				grid.OnMouseClick(x, y);
			}
			
		}

		SDL_SetRenderDrawColor(GameManager::Get()->GetRenderer(), 255, 255, 255, 255);
		SDL_RenderClear(GameManager::Get()->GetRenderer());

		grid.Render();

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (grid.grid[i][j] == 1)
				{
					GameManager::Get()->GetXTexture()->Render(x - 100 / 2, y - 100 / 2);
				}
			}
		}

		//GameManager::Get()->GetXTexture()->Render(x - 100 / 2, y - 100 / 2);

		SDL_RenderPresent(GameManager::Get()->GetRenderer());
	}

	//Free resources and close SDL
	GameManager::Get()->Destroy();
	GameManager::UnregisterInstance(gameManager);

	return 0;
}
