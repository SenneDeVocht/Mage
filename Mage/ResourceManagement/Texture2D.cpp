#include "Mage/MagePCH.h"
#include "Texture2D.h"

Mage::Texture2D::Texture2D(SDL_Texture* texture, float pixelsPerUnit, float pivotX, float pivotY)
	: m_Texture{ texture}
	, m_Pivot{ pivotX, pivotY }
	, m_PixelsPerUnit{ pixelsPerUnit }
{}

Mage::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_Texture);
}

SDL_Texture* Mage::Texture2D::GetSDLTexture() const
{
	return m_Texture;
}

const glm::vec2& Mage::Texture2D::GetPivot() const
{
	return m_Pivot;
}

float Mage::Texture2D::GetPixelsPerUnit() const
{
	return m_PixelsPerUnit;
}
