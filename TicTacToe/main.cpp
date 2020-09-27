#include <SDL.h>

#include <stdio.h>

namespace tickTacToe
{
	const int screenWidth = 720;
	const int screenHeight = 480;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	bool Initialize()
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
		window = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			return false;
		}

		//Create vsynced renderer for window
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (renderer == NULL)
		{
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			return false;
		}

		//Initialize renderer color
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		return true;
	}

	void Destroy()
	{
		SDL_DestroyWindow(window);
		window = nullptr;

		SDL_DestroyRenderer(renderer);
		renderer = nullptr;

		SDL_Quit();
	}

	void DrawGrid()
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 139, 255);
		int offset = 100;
		int width = screenWidth - 2 * offset;
		int height = 5;

		//Draw grid using rectangles. We make the line bolder this way.
		SDL_Rect firstHorizontalLine = { offset, screenHeight / 4, width, height };
		SDL_RenderFillRect(renderer, &firstHorizontalLine);

		SDL_Rect secondHorizontalLine = { offset, screenHeight / 3 + offset, width, height };
		SDL_RenderFillRect(renderer, &secondHorizontalLine);

		SDL_Rect firstVerticallLine = { static_cast<int>(2.5 * offset), screenHeight / offset, height, width - static_cast<int>(1.5 * offset) };
		SDL_RenderFillRect(renderer, &firstVerticallLine);

		SDL_Rect secondVerticalLine = { static_cast<int>(4.5 * offset), screenHeight / offset, height , width - static_cast<int>(1.5 * offset) };
		SDL_RenderFillRect(renderer, &secondVerticalLine);

		//Draw grid using only lines
		//////Hozrizontal lines
		//SDL_RenderDrawLine(renderer, offset, screenHeight / 4, screenWidth - offset, screenHeight / 4);
		//SDL_RenderDrawLine(renderer, offset, screenHeight / 3 + offset, screenWidth - offset, screenHeight / 3 + offset);
		////Vertical lines
		//SDL_RenderDrawLine(renderer, 2.5 * offset, screenHeight / offset, 2.5 * offset, screenHeight - offset);
		//SDL_RenderDrawLine(renderer, 4.5 * offset, screenHeight / offset, 4.5 * offset, screenHeight - offset);
	}
}

int main(int argc, char* args[])
{
	if (!tickTacToe::Initialize())
	{
		printf("Failed to initialize!\n");
		return -1;
	}

	bool quit = false;

	//Event handler
	SDL_Event e;

	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		SDL_SetRenderDrawColor(tickTacToe::renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(tickTacToe::renderer);

		tickTacToe::DrawGrid();

		SDL_RenderPresent(tickTacToe::renderer);
	}

	//Free resources and close SDL
	tickTacToe::Destroy();

	return 0;
}
