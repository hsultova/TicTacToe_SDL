#include "Grid.h"

#include <SDL.h>

#include "GameManager.h"
#include "Core/Texture.h"

Grid::Grid()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			grid[i][j] = Cell();
		}
	}
}

void Grid::Render()
{
	SDL_SetRenderDrawColor(GameManager::Get()->GetRenderer(), m_gridColor.Red, m_gridColor.Green, m_gridColor.Blue, m_gridColor.Alpha);
	int lineWidth = GameManager::Get()->GetScreenWidth() - 2 * m_cellSize;

	//Draw grid using rectangles
	SDL_Rect firstHorizontalLine = { m_cellSize, GameManager::Get()->GetScreenHeight() / 4, lineWidth, m_borderThickness };
	SDL_RenderFillRect(GameManager::Get()->GetRenderer(), &firstHorizontalLine);

	SDL_Rect secondHorizontalLine = { m_cellSize, GameManager::Get()->GetScreenHeight() / 3 + m_cellSize, lineWidth, m_borderThickness };
	SDL_RenderFillRect(GameManager::Get()->GetRenderer(), &secondHorizontalLine);

	SDL_Rect firstVerticallLine = { static_cast<int>(2.5 * m_cellSize), GameManager::Get()->GetScreenHeight() / m_cellSize, m_borderThickness, lineWidth - static_cast<int>(1.5 * m_cellSize) };
	SDL_RenderFillRect(GameManager::Get()->GetRenderer(), &firstVerticallLine);

	SDL_Rect secondVerticalLine = { static_cast<int>(4.5 * m_cellSize), GameManager::Get()->GetScreenHeight() / m_cellSize, m_borderThickness , lineWidth - static_cast<int>(1.5 * m_cellSize) };
	SDL_RenderFillRect(GameManager::Get()->GetRenderer(), &secondVerticalLine);
}

void Grid::OnMouseClick(int _x, int _y)
{
	//get current player symbol


	int x = _x / (2 * m_cellSize);
	int y = (_y + m_cellSize) / (2 * m_cellSize);

	if (x < 0 || x >= 3 || y < 0 || y >= 3)
	{
		//Out of array bounds
		return;
	}

	Player player = GameManager::Get()->GetCurrentPlayer();

	Cell cell = grid[x][y];
	cell.SetLocalPosition(Position{ x,y });
	cell.SetGlobalPosition(Position{ _x - m_cellSize / 2, _y - m_cellSize / 2 });
	cell.SetSymbol(player.mark);
	grid[x][y] = cell;

	GameManager::Get()->SwitchPlayer();
}
