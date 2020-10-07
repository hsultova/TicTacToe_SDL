#pragma once
#include "Cell.h"

class Grid
{
public:
	Cell grid[3][3];

	Grid();
	~Grid();

	const int GetCellSize() const;
	const int GetBorderThickness() const;

	void OnMouseClick(int _x, int _y);
	void Clear();

private:
	int m_cellSize;
	const int m_borderThickness = 5;
};

