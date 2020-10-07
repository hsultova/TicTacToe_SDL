#include "Grid.h"

#include <SDL.h>
#include <assert.h> 

#include "GameManager.h"
#include "..\Core\Texture.h"

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
	//TODO Draw grid based on cells?
	SDL_Renderer* renderer = GameManager::Get()->GetRenderer();
	assert(renderer != nullptr);

	SDL_SetRenderDrawColor(renderer, m_gridColor.r, m_gridColor.g, m_gridColor.b, m_gridColor.a);
	m_lineWidth = GameManager::Get()->GetScreenWidth() - 2 * m_cellSize;

	m_firstHorizontalLinePosition = Position{ m_cellSize ,  GameManager::Get()->GetScreenHeight() / 4 };
	//Draw grid using rectangles
	SDL_Rect firstHorizontalLine = { m_firstHorizontalLinePosition.x, m_firstHorizontalLinePosition.y, m_lineWidth, m_borderThickness };
	SDL_RenderFillRect(renderer, &firstHorizontalLine);

	m_secondHorizontalLinePosition = Position{ m_cellSize , GameManager::Get()->GetScreenHeight() / 3 + m_cellSize };
	SDL_Rect secondHorizontalLine = { m_secondHorizontalLinePosition.x, m_secondHorizontalLinePosition.y, m_lineWidth, m_borderThickness };
	SDL_RenderFillRect(renderer, &secondHorizontalLine);

	m_firstVerticalLinePosition = Position{ static_cast<int>(2.5 * m_cellSize) ,  GameManager::Get()->GetScreenHeight() / m_cellSize };
	SDL_Rect firstVerticallLine = { m_firstVerticalLinePosition.x, m_firstVerticalLinePosition.y, m_borderThickness, m_lineWidth - static_cast<int>(1.5 * m_cellSize) };
	SDL_RenderFillRect(renderer, &firstVerticallLine);

	m_secondVerticalLinePosition = Position{ static_cast<int>(4.5 * m_cellSize) ,  GameManager::Get()->GetScreenHeight() / m_cellSize };
	SDL_Rect secondVerticalLine = { m_secondVerticalLinePosition.x, m_secondVerticalLinePosition.y, m_borderThickness , m_lineWidth - static_cast<int>(1.5 * m_cellSize) };
	SDL_RenderFillRect(renderer, &secondVerticalLine);
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

Position Grid::GetMinPosition()
{
	return Position{ m_firstHorizontalLinePosition.x, m_firstHorizontalLinePosition.y - m_cellSize };
}

Position Grid::GetMaxPosition()
{
	return Position{ m_secondHorizontalLinePosition.x + m_lineWidth, m_secondHorizontalLinePosition.y + m_cellSize };
}

