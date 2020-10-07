#include "Grid.h"

#include <assert.h> 

#include "GameManager.h"

Grid::Grid()
{
	//Initialize the grid with empty cells by default
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			grid[i][j] = Cell();
		}
	}

	m_cellSize = GameManager::Get()->GetWindowWidth() / 7;
}

Grid::~Grid()
{
}

const int Grid::GetCellSize() const
{
	return m_cellSize;
}

const int Grid::GetBorderThickness() const
{
	return m_borderThickness;
}

void Grid::OnMouseClick(int _x, int _y)
{
	//Convert global position (mouse coordinates) to local position (grid ids)
	//TODO Extract to method
	int x = _x / (2 * m_cellSize + m_cellSize / 4) ;
	int y = (_y + m_cellSize) / (2 * m_cellSize );

	if (x < 0 || x >= 3 || y < 0 || y >= 3)
	{
		//Out of array bounds
		//TODO log error
		return;
	}

	Cell cell = grid[x][y];
	cell.SetLocalPosition(Position{ x, y });
	cell.SetGlobalPosition(Position{ m_cellSize + x * 2 * m_cellSize, m_cellSize / 5 + y * (m_cellSize + m_cellSize / 4) });
	if (cell.GetSymbol() == Symbol::e)
	{
		cell.SetSymbol(GameManager::Get()->GetCurrentPlayer().mark);
		grid[x][y] = cell;
		GameManager::Get()->ChangeTurn();
	}
}

void Grid::Clear()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			grid[i][j].SetSymbol(Symbol::e);
		}
	}
}

void Grid::UpdateCellSize()
{
	m_cellSize = GameManager::Get()->GetWindowWidth() / 7;
}
