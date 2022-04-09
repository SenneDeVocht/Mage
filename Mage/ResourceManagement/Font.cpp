#include "Mage/MagePCH.h"
#include "Font.h"

#include <SDL_ttf.h>

Mage::Font::Font(const std::string& fullPath, unsigned int pointHeight)
	: m_Font(nullptr)
	, m_Size(pointHeight)
{
	m_Font = TTF_OpenFont(fullPath.c_str(), pointHeight);
	if (m_Font == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
	}
}

Mage::Font::~Font()
{
	TTF_CloseFont(m_Font);
}

TTF_Font* Mage::Font::GetFont() const {
	return m_Font;
}

