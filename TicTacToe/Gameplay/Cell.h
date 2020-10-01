#pragma once

enum class Symbol
{
	//empty
	e,
	//x texture
	x,
	//o texture
	o
};

struct Position
{
	int x = 0;
	int y = 0;
};

class Cell
{
public:
	void SetLocalPosition(const Position& _position);
	void SetGlobalPosition(const Position& _position);
	void SetSymbol(const Symbol _symbol);

	Position GetLocalPosition() const;
	Position GetGlobalPosition() const;
	Symbol GetSymbol() const;

private:
	Position m_localPosition;
	Position m_globalPosition;
	Symbol m_symbol = Symbol::e;
};

