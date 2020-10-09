#include "GameManager.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h>

GameManager* GameManager::s_instance = nullptr;

GameManager* GameManager::Get()
{
	return s_instance;
}

void GameManager::RegisterInstance(GameManager* _instance)
{
	assert(s_instance == nullptr);
	s_instance = _instance;
}

void GameManager::UnregisterInstance(GameManager* _instance)
{
	assert(s_instance == _instance);
	s_instance = nullptr;
}

SDL_Window* GameManager::GetWindow() const
{
	return m_window;
}

SDL_Renderer* GameManager::GetRenderer() const
{
	return m_renderer;
}

bool GameManager::Initialize()
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
	m_window = SDL_CreateWindow("Tic Tac Toe",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		m_windowWidth,
		m_windowHeight,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if (m_window == nullptr)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	SDL_SetWindowMinimumSize(m_window, 720, 480);

	//Create vsynced renderer for window
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_renderer == nullptr)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

	if (IMG_Init(IMG_INIT_JPG) < 0)
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}

	//Initialize class members
	m_oTexture = new Texture();
	m_xTexture = new Texture();
	m_oColorTexture = new Texture();
	m_xColorTexture = new Texture();
	m_textTexture = new Texture();
	m_restartGameTexture = new Texture();
	m_restartGameButtonPosition = Position{ GetWindowWidth() - 300 , GetWindowHeight() - 100 };

	m_grid = new Grid();

	m_oPlayerColor = SDL_Color{ 157, 7, 8, 255 };
	m_xPlayerColor = SDL_Color{ 11, 113, 100, 255 };
	m_mainColor = SDL_Color{ 0, 48, 119, 255 };
	m_gridColor = SDL_Color{ 0, 0, 139, 255 };

	m_oPlayer = Player{ "O", Symbol::o, m_oPlayerColor };
	m_xPlayer = Player{ "X", Symbol::x, m_xPlayerColor };
	m_currentPlayer = m_xPlayer;

	return true;
}

void GameManager::Destroy()
{
	delete m_oTexture;
	delete m_xTexture;
	delete m_oColorTexture;
	delete m_xColorTexture;
	delete m_textTexture;
	delete m_restartGameTexture;
	delete m_grid;

	SDL_DestroyWindow(m_window);
	m_window = nullptr;

	SDL_DestroyRenderer(m_renderer);
	m_renderer = nullptr;

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

const int GameManager::GetWindowWidth() const
{
	return m_windowWidth;
}

const int GameManager::GetWindowHeight() const
{
	return m_windowHeight;
}

Player GameManager::GetCurrentPlayer() const
{
	return m_currentPlayer;
}

void GameManager::PlayGame()
{
	LoadTextures();
	m_textTexture->SetFont(TTF_OpenFont("Textures/Lovely_Kids.ttf", 64));
	m_textTexture->LoadFromRenderedText("X Turn", m_xPlayerColor);

	bool quit = false;
	m_hasWinner = false;
	m_endGame = false;
	//Event handler
	SDL_Event e;
	SDL_Cursor* cursor;

	assert(m_grid != nullptr);

	while (!quit)
	{
		//Input events
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			if (e.type == SDL_WINDOWEVENT)
			{
				SDL_GetWindowSize(m_window, &m_windowWidth, &m_windowHeight);
				m_grid->UpdateCellSize();
				UpdateButtons();
			}

			if (e.type == SDL_MOUSEMOTION)
			{
				int x = 0;
				int y = 0;
				SDL_GetMouseState(&x, &y);

				if (CheckBounds(
					Position{ x, y },
					m_restartGameButtonPosition,
					Position{ m_restartGameButtonPosition.x + m_restartGameTexture->GetWidth(), m_restartGameButtonPosition.y + m_restartGameTexture->GetHeight() }))
				{
					cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
				}
				else
				{
					cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
				}

				SDL_SetCursor(cursor);
			}

			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				int x = 0;
				int y = 0;
				SDL_GetMouseState(&x, &y);

				if (CheckBounds(Position{ x, y },
					m_restartGameButtonPosition,
					Position{ m_restartGameButtonPosition.x + m_restartGameTexture->GetWidth(), m_restartGameButtonPosition.y + m_restartGameTexture->GetHeight() }))
				{
					RestartGame();
				}

				if (m_endGame == false && CheckBounds(Position{ x, y }, GetGridMinPosition(), GetGridMaxPosition()))
				{
					m_grid->OnMouseClick(x, y);
				}
			}
		}

		//Game Logic
		GameState gameState = CheckVictory();
		if (gameState != GameState::inProgress)
		{
			m_textTexture->LoadFromRenderedText(GetGameState(gameState), m_mainColor);

			if (gameState != GameState::draw)
			{
				m_hasWinner = true;
			}
			m_endGame = true;
		}

		//Rendering
		SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
		SDL_RenderClear(m_renderer);

		m_textTexture->Render(100, GetWindowHeight() - 100);
		m_restartGameTexture->Render(m_restartGameButtonPosition.x, m_restartGameButtonPosition.y);

		RenderGrid();
		RenderGridCells();

		if (m_hasWinner == true)
		{
			//RenderLine();
			RenderColorTexture();
		}

		SDL_RenderPresent(m_renderer);
	}
}

void GameManager::RenderColorTexture()
{
	if (m_currentPlayer.mark == Symbol::o)
	{
		m_xColorTexture->Render(m_line.start.x, m_line.start.y);
		m_xColorTexture->Render(m_line.middle.x, m_line.middle.y);
		m_xColorTexture->Render(m_line.end.x, m_line.end.y);
	}
	else
	{
		m_oColorTexture->Render(m_line.start.x, m_line.start.y);
		m_oColorTexture->Render(m_line.middle.x, m_line.middle.y);
		m_oColorTexture->Render(m_line.end.x, m_line.end.y);
	}
}

void GameManager::RenderLine()
{
	SDL_SetRenderDrawColor(m_renderer, m_mainColor.r, m_mainColor.g, m_mainColor.b, m_mainColor.a);

	int offset = 25;
	int x = m_line.start.x + offset;
	int y = m_line.start.y + offset;
	while (true)
	{
		SDL_Rect rect = { x, y , 25, 20 };
		SDL_RenderFillRect(m_renderer, &rect);
		if (x <= m_line.end.x + offset)
		{
			++x;
		}

		if (y <= m_line.end.y + offset)
		{
			++y;
		}

		if (x > m_line.end.x + offset && y > m_line.end.y + offset)
		{
			break;
		}
	}
}

void GameManager::RenderGridCells()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Cell cell = m_grid->grid[i][j];
			if (cell.GetSymbol() == Symbol::o)
			{
				m_oTexture->Render(cell.GetGlobalPosition().x, cell.GetGlobalPosition().y);
			}
			else if (cell.GetSymbol() == Symbol::x)
			{
				m_xTexture->Render(cell.GetGlobalPosition().x, cell.GetGlobalPosition().y);
			}
		}
	}
}

void GameManager::RenderGrid()
{
	//TODO Draw grid based on cells?

	SDL_SetRenderDrawColor(m_renderer, m_gridColor.r, m_gridColor.g, m_gridColor.b, m_gridColor.a);
	m_lineWidth = m_windowWidth - 2 * m_grid->GetCellSize();

	m_firstHorizontalLinePosition = Position{ m_grid->GetCellSize() ,  m_windowHeight / 4 };
	//Draw grid using rectangles
	SDL_Rect firstHorizontalLine = { 
		m_firstHorizontalLinePosition.x, 
		m_firstHorizontalLinePosition.y,
		m_lineWidth,
		m_grid->GetBorderThickness() };

	SDL_RenderFillRect(m_renderer, &firstHorizontalLine);

	m_secondHorizontalLinePosition = Position{ 
		m_grid->GetCellSize() , 
		m_windowHeight / 3 + m_grid->GetCellSize() };

	SDL_Rect secondHorizontalLine = {
		m_secondHorizontalLinePosition.x, 
		m_secondHorizontalLinePosition.y, 
		m_lineWidth,
		m_grid->GetBorderThickness() };
	SDL_RenderFillRect(m_renderer, &secondHorizontalLine);

	m_firstVerticalLinePosition = Position{
		static_cast<int>(2.5 * m_grid->GetCellSize()) , 
		m_windowHeight/ m_grid->GetCellSize() };

	SDL_Rect firstVerticallLine = {
		m_firstVerticalLinePosition.x,
		m_firstVerticalLinePosition.y, 
		m_grid->GetBorderThickness(),
		m_lineWidth - static_cast<int>(1.5 * m_grid->GetCellSize()) };
	SDL_RenderFillRect(m_renderer, &firstVerticallLine);

	m_secondVerticalLinePosition = Position{
		static_cast<int>(4.5 * m_grid->GetCellSize()) , 
		m_windowHeight / m_grid->GetCellSize() };

	SDL_Rect secondVerticalLine = { 
		m_secondVerticalLinePosition.x,
		m_secondVerticalLinePosition.y, 
		m_grid->GetBorderThickness() , 
		m_lineWidth - static_cast<int>(1.5 * m_grid->GetCellSize()) };
	SDL_RenderFillRect(m_renderer, &secondVerticalLine);
}

void GameManager::SwitchPlayer()
{
	if (m_currentPlayer.mark == Symbol::x)
	{
		m_currentPlayer = m_oPlayer;
	}
	else
	{
		m_currentPlayer = m_xPlayer;
	}
}

void GameManager::LoadTextures()
{
	if (m_oTexture == nullptr || m_xTexture == nullptr)
		return;

	m_xTexture->LoadFromFile("Textures/x.JPG");
	m_oTexture->LoadFromFile("Textures/o.JPG");
	m_xColorTexture->LoadFromFile("Textures/x_color.JPG");
	m_oColorTexture->LoadFromFile("Textures/o_color.JPG");
	m_restartGameTexture->LoadFromFile("Textures/restart_game.JPG");
}

void GameManager::ChangeTurn()
{
	SwitchPlayer();
	m_textTexture->LoadFromRenderedText(m_currentPlayer.name + " Turn", m_currentPlayer.color);
}

Player GameManager::GetPlayer(const Symbol _symbol)
{
	if (_symbol == Symbol::x)
	{
		return m_xPlayer;
	}
	else
	{
		return m_oPlayer;
	}
}

Position GameManager::GetGridMinPosition()
{
	return Position{
		m_firstHorizontalLinePosition.x,
		m_firstHorizontalLinePosition.y - m_grid->GetCellSize() };
}

Position GameManager::GetGridMaxPosition()
{
	return Position{
		m_secondHorizontalLinePosition.x + m_lineWidth, 
		m_secondHorizontalLinePosition.y + m_grid->GetCellSize() };
}

void GameManager::UpdateButtons()
{
	m_restartGameButtonPosition = Position{ GetWindowWidth() - 300 ,GetWindowHeight() - 100 };
}

bool GameManager::CheckBounds(const Position& _position, const Position& _minPosition, const Position& _maxPosition)
{
	return (_position.x < _maxPosition.x&& _position.x > _minPosition.x)
		&& (_position.y < _maxPosition.y&& _position.y > _minPosition.y);
}

GameState GameManager::CheckVictory()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_grid->grid[i][0].GetSymbol() != Symbol::e
			&& m_grid->grid[i][0].GetSymbol() == m_grid->grid[i][1].GetSymbol()
			&& m_grid->grid[i][1].GetSymbol() == m_grid->grid[i][2].GetSymbol())
		{
			m_line = Line{ m_grid->grid[i][0].GetGlobalPosition(), m_grid->grid[i][1].GetGlobalPosition(), m_grid->grid[i][2].GetGlobalPosition() };
			if (m_grid->grid[i][0].GetSymbol() == Symbol::x)
			{
				return GameState::xWon;
			}
			else
			{
				return GameState::oWon;
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (m_grid->grid[0][i].GetSymbol() != Symbol::e
			&& m_grid->grid[0][i].GetSymbol() == m_grid->grid[1][i].GetSymbol()
			&& m_grid->grid[1][i].GetSymbol() == m_grid->grid[2][i].GetSymbol())
		{
			m_line = Line{ m_grid->grid[0][i].GetGlobalPosition(), m_grid->grid[1][i].GetGlobalPosition(), m_grid->grid[2][i].GetGlobalPosition() };
			if (m_grid->grid[0][i].GetSymbol() == Symbol::x)
			{
				return GameState::xWon;
			}
			else
			{
				return GameState::oWon;
			}
		}
	}

	if (m_grid->grid[0][0].GetSymbol() != Symbol::e
		&& m_grid->grid[0][0].GetSymbol() == m_grid->grid[1][1].GetSymbol()
		&& m_grid->grid[1][1].GetSymbol() == m_grid->grid[2][2].GetSymbol())
	{
		m_line = Line{ m_grid->grid[0][0].GetGlobalPosition(), m_grid->grid[1][1].GetGlobalPosition(), m_grid->grid[2][2].GetGlobalPosition() };
		if (m_grid->grid[0][0].GetSymbol() == Symbol::x)
		{
			return GameState::xWon;
		}
		else
		{
			return GameState::oWon;
		}
	}

	if (m_grid->grid[0][2].GetSymbol() != Symbol::e
		&& m_grid->grid[0][2].GetSymbol() == m_grid->grid[1][1].GetSymbol()
		&& m_grid->grid[1][1].GetSymbol() == m_grid->grid[2][0].GetSymbol())
	{
		m_line = Line{ m_grid->grid[0][2].GetGlobalPosition(), m_grid->grid[1][1].GetGlobalPosition(), m_grid->grid[2][0].GetGlobalPosition() };
		if (m_grid->grid[0][2].GetSymbol() == Symbol::x)
		{
			return GameState::xWon;
		}
		else
		{
			return GameState::oWon;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (m_grid->grid[i][j].GetSymbol() == Symbol::e)
			{
				return GameState::inProgress;
			}
		}
	}

	return GameState::draw;
}

void GameManager::RestartGame()
{
	m_endGame = false;
	m_hasWinner = false;
	m_grid->Clear();
	m_currentPlayer = m_xPlayer;
	m_textTexture->LoadFromRenderedText(m_currentPlayer.name + " Turn", m_currentPlayer.color);
}

const std::string GameManager::GetGameState(const GameState& _gameState) const
{
	switch (_gameState)
	{
	case GameState::draw:
		return "Draw";
	case GameState::inProgress:
		return "In Progress";
	case GameState::oWon:
		return "O Won";
	case GameState::xWon:
		return "X Won";
	}

	return std::string();
}
