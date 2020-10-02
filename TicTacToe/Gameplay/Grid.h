#pragma once
#include "Cell.h"

struct GridColor
{
	int Red;
	int Green;
	int Blue;
	int Alpha;
};

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

	GridColor m_gridColor = GridColor{ 0, 0, 139, 255 };
};

