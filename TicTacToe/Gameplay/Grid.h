#pragma once
#include <SDL.h>

#include "Cell.h"

class Grid
{
public:
	Cell grid[3][3];

	Grid();
	~Grid();

	void Render();
	void OnMouseClick(int _x, int _y);
	void Clear();

private:
	const int m_cellSize = 100;
	const int m_borderThickness = 5;

	SDL_Color m_gridColor = SDL_Color{ 0, 0, 139, 255 };
};

