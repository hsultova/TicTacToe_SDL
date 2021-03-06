#include "Texture.h"

#include <SDL_image.h>

#include "GameManager.h"

Texture::Texture()
{
	m_texture = nullptr;
	m_font = nullptr;
	m_width = 0;
	m_height = 0;
}

Texture::Texture(SDL_Texture* _texture, int _width, int _height)
	: m_texture(_texture), m_width(_width), m_height(_height)
{
}

Texture::~Texture()
{
	Free();
}

void Texture::Free()
{
	if (m_texture != nullptr)
	{
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;

		TTF_CloseFont(m_font);
		m_font = nullptr;

		m_width = 0;
		m_height = 0;
	}
}

bool Texture::LoadFromFile(std::string _path)
{
	SDL_Texture* newTexture = nullptr;

	SDL_Surface* loadedSurface = IMG_Load(_path.c_str());
	if (loadedSurface == nullptr)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", _path.c_str(), IMG_GetError());
		return false;
	}
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 255, 255));

	assert(GameManager::Get()->GetRenderer() != nullptr);

	newTexture = SDL_CreateTextureFromSurface(GameManager::Get()->GetRenderer(), loadedSurface);
	if (newTexture == nullptr)
	{
		printf("Unable to create texture from %s! SDL Error: %s\n", _path.c_str(), SDL_GetError());
	}

	m_width = loadedSurface->w;
	m_height = loadedSurface->h;

	SDL_FreeSurface(loadedSurface);
	m_texture = newTexture;

	return m_texture != nullptr;
}

bool Texture::LoadFromRenderedText(std::string _text, SDL_Color _color)
{
	SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, _text.c_str(), _color);
	if (textSurface == nullptr)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}

	assert(GameManager::Get()->GetRenderer() != nullptr);
	m_texture = SDL_CreateTextureFromSurface(GameManager::Get()->GetRenderer(), textSurface);
	if (m_texture == nullptr)
	{
		printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	m_width = textSurface->w;
	m_height = textSurface->h;

	SDL_FreeSurface(textSurface);

	return m_texture != nullptr;
}

void Texture::SetColor(Uint8 _red, Uint8 _green, Uint8 _blue)
{
	SDL_SetTextureColorMod(m_texture, _red, _green, _blue);
}

void Texture::SetBlendMode(SDL_BlendMode _blendMode)
{
	SDL_SetTextureBlendMode(m_texture, _blendMode);
}

void Texture::SetAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(m_texture, alpha);
}

bool Texture::SetFont(TTF_Font* _font)
{
	if (_font == nullptr)
	{
			printf("Failed to load the font! SDL_ttf Error: %s\n", TTF_GetError());
			return false;
	}
	m_font = _font;

	return true;
}

void Texture::Render(int x, int y, SDL_Rect* clip)
{
	SDL_Rect renderRect = { x, y, m_width, m_height };
	if (clip != nullptr)
	{
		renderRect.w = clip->w;
		renderRect.h = clip->h;
	}

	assert(GameManager::Get()->GetRenderer() != nullptr);
	SDL_RenderCopy(GameManager::Get()->GetRenderer(), m_texture, clip, &renderRect);
}

int Texture::GetWidth() const
{
	return m_width;
}

int Texture::GetHeight() const
{
	return m_height;
}
