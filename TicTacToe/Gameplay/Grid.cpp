#include "Grid.h"

#include <SDL.h>
#include <assert.h> 

#include "GameManager.h"
#include "Core/Texture.h"

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
}

Grid::~Grid()
{
}

void Grid::Render()
{
	SDL_Renderer* renderer = GameManager::Get()->GetRenderer();
	assert(renderer != nullptr);

	SDL_SetRenderDrawColor(renderer, m_gridColor.Red, m_gridColor.Green, m_gridColor.Blue, m_gridColor.Alpha);
	int lineWidth = GameManager::Get()->GetScreenWidth() - 2 * m_cellSize;

	//Draw grid using rectangles
	SDL_Rect firstHorizontalLine = { m_cellSize, GameManager::Get()->GetScreenHeight() / 4, lineWidth, m_borderThickness };
	SDL_RenderFillRect(renderer, &firstHorizontalLine);

	SDL_Rect secondHorizontalLine = { m_cellSize, GameManager::Get()->GetScreenHeight() / 3 + m_cellSize, lineWidth, m_borderThickness };
	SDL_RenderFillRect(renderer, &secondHorizontalLine);

	SDL_Rect firstVerticallLine = { static_cast<int>(2.5 * m_cellSize), GameManager::Get()->GetScreenHeight() / m_cellSize, m_borderThickness, lineWidth - static_cast<int>(1.5 * m_cellSize) };
	SDL_RenderFillRect(renderer, &firstVerticallLine);

	SDL_Rect secondVerticalLine = { static_cast<int>(4.5 * m_cellSize), GameManager::Get()->GetScreenHeight() / m_cellSize, m_borderThickness , lineWidth - static_cast<int>(1.5 * m_cellSize) };
	SDL_RenderFillRect(renderer, &secondVerticalLine);
}

void Grid::OnMouseClick(int _x, int _y)
{
	//Convert global position (mouse coordinates) to local position (grid ids)
	//TODO Extract to method
	int x = _x / (2 * m_cellSize);
	int y = (_y + m_cellSize) / (2 * m_cellSize);

	if (x < 0 || x >= 3 || y < 0 || y >= 3)
	{
		//Out of array bounds
		//TODO log error
		return;
	}

	Cell cell = grid[x][y];
	cell.SetLocalPosition(Position{ x, y });
	cell.SetGlobalPosition(Position{ _x - m_cellSize / 2, _y - m_cellSize / 2 });
	cell.SetSymbol(GameManager::Get()->GetCurrentPlayer().mark);
	grid[x][y] = cell;

	GameManager::Get()->SwitchPlayer();
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
