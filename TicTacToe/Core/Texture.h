#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

#include <string>

class Texture
{
public:
	Texture();
	Texture(SDL_Texture* _texture, int _width, int _height);
	~Texture();

	void Free();
	bool LoadFromFile(std::string _path);
	bool LoadFromRenderedText(std::string _text, SDL_Color _color);
	void SetColor(Uint8 _red, Uint8 _green, Uint8 _blue);
	void SetBlendMode(SDL_BlendMode _blendMode);
	void SetAlpha(Uint8 alpha);
	bool SetFont(TTF_Font* _font);
	void Render(int x, int y, SDL_Rect* clip = nullptr);

	int GetWidth() const;
	int GetHeight() const;

private:
	SDL_Texture* m_texture;
	TTF_Font* m_font = nullptr;

	int m_width;
	int m_height;
};

