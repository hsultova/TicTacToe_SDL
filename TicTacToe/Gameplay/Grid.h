#pragma once
#include "Cell.h"

struct GridColor
{
	int Red = 0;
	int Green = 0;
	int Blue = 139;
	int Alpha = 255;
};

class Grid
{
public:
	Cell grid[3][3];

	Grid();

	void Render();
	void OnMouseClick(int _x, int _y);

private:
	const int m_cellSize = 100;
	const int m_borderThickness = 5;

	GridColor m_gridColor;
};

