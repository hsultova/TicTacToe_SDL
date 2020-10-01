#include "Cell.h"

void Cell::SetLocalPosition(const Position& _position)
{
    m_localPosition = _position;
}

void Cell::SetGlobalPosition(const Position& _position)
{
    m_globalPosition = _position;
}

void Cell::SetSymbol(const Symbol _symbol)
{
    m_symbol = _symbol;
}

Position Cell::GetLocalPosition() const
{
    return m_localPosition;
}

Position Cell::GetGlobalPosition() const
{
    return m_globalPosition;
}

Symbol Cell::GetSymbol() const
{
    return m_symbol;
}
