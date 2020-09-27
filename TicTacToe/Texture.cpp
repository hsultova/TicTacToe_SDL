#include "Texture.h"

Texture::Texture()
{
	m_texture = nullptr;
	m_width = 0;
	m_height = 0;
}

Texture::Texture(SDL_Texture* _texture, int _width, int _height) 
	: m_texture(_texture), m_width(_width), m_height(_height)
{
}

Texture::~Texture()
{
}

bool Texture::LoadFromFile(std::string _path)
{
	return false;
}

void Texture::SetColor(Uint8 _red, Uint8 _green, Uint8 _blue)
{
}

void Texture::SetBlendMode(SDL_BlendMode _blendMode)
{
}

void Texture::SetAlpha(Uint8 alpha)
{
}

void Texture::Render(int x, int y, SDL_Rect* clip)
{
}

int Texture::GetWidth() const
{
	return 0;
}

int Texture::GetHeight() const
{
	return 0;
}
