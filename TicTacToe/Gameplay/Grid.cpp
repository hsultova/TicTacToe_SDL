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
			grid[i][j] = 0;
		}
	}
}

void Grid::Render()
{
	SDL_SetRenderDrawColor(GameManager::Get()->GetRenderer(), m_gridColor.Red, m_gridColor.Green, m_gridColor.Blue, m_gridColor.Alpha);
	int lineWidth = GameManager::Get()->GetScreenWidth() - 2 * m_cellWidth;

	//Draw grid using rectangles
	SDL_Rect firstHorizontalLine = { m_cellWidth, GameManager::Get()->GetScreenHeight() / 4, lineWidth, m_borderThickness };
	SDL_RenderFillRect(GameManager::Get()->GetRenderer(), &firstHorizontalLine);

	SDL_Rect secondHorizontalLine = { m_cellWidth, GameManager::Get()->GetScreenHeight() / 3 + m_cellWidth, lineWidth, m_borderThickness };
	SDL_RenderFillRect(GameManager::Get()->GetRenderer(), &secondHorizontalLine);

	SDL_Rect firstVerticallLine = { static_cast<int>(2.5 * m_cellWidth), GameManager::Get()->GetScreenHeight() / m_cellWidth, m_borderThickness, lineWidth - static_cast<int>(1.5 * m_cellWidth) };
	SDL_RenderFillRect(GameManager::Get()->GetRenderer(), &firstVerticallLine);

	SDL_Rect secondVerticalLine = { static_cast<int>(4.5 * m_cellWidth), GameManager::Get()->GetScreenHeight() / m_cellWidth, m_borderThickness , lineWidth - static_cast<int>(1.5 * m_cellWidth) };
	SDL_RenderFillRect(GameManager::Get()->GetRenderer(), &secondVerticalLine);
}

void Grid::OnMouseClick(int _x, int _y)
{
	//get current player symbol
	
	grid[_x / m_cellWidth][ _y / m_cellWidth] = 1;
}
