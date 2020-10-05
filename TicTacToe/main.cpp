#include <stdio.h>

#include <assert.h> 

#include "Gameplay\GameManager.h"
#include "Core\Texture.h"
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
	GameManager::Get()->GetTextTexture()->SetFont(TTF_OpenFont("Textures/Lovely_Kids.ttf", 64));
	GameManager::Get()->GetTextTexture()->LoadFromRenderedText("X Turn", GameManager::Get()->GetXPlayerColor());

	bool quit = false;
	bool renderLine = false;
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
		GameState gameState = GameManager::Get()->CheckVictory();
		if (gameState != GameState::inProgress)
		{
			GameManager::Get()->GetTextTexture()->LoadFromRenderedText(GameManager::Get()->GetGameState(gameState), GameManager::Get()->GetMainColor());

			if (gameState != GameState::draw)
			{
				renderLine = true;
			}

			//grid->Clear();
		}

		//Rendering
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		GameManager::Get()->GetTextTexture()->Render(100, GameManager::Get()->GetScreenHeight() - 100);
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

		if (renderLine == true)
		{
			Line line = GameManager::Get()->GetLine();
			SDL_Color color = GameManager::Get()->GetMainColor();
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

			int offset = 25;
			int x = line.start.x + offset;
			int y = line.start.y + offset;
			while (true)
			{
				SDL_Rect rect = { x, y , 25, 20 };
				SDL_RenderFillRect(renderer, &rect);
				if (x <= line.end.x + offset)
				{
					++x;
				}

				if (y <= line.end.y + offset)
				{
					++y;
				}

				if (x > line.end.x + offset && y > line.end.y + offset)
				{
					break;
				}
			}

			//SDL_RenderDrawLine(renderer, line.start.x, line.start.y, line.end.x, line.end.y);
		}

		SDL_RenderPresent(renderer);
	}

	//Free resources and close SDL
	GameManager::Get()->Destroy();
	GameManager::UnregisterInstance(gameManager);

	return 0;
}
