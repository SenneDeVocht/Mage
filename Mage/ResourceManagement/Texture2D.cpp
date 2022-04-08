#include "Mage/MagePCH.h"
#include "Texture2D.h"

Mage::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_Texture = texture;
}

Mage::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_Texture);
}

SDL_Texture* Mage::Texture2D::GetSDLTexture() const
{
	return m_Texture;
}