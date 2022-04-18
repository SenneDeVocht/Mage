#include "Mage/MagePCH.h"
#include "ResourceManager.h"

#pragma warning(push, 0)
#include <SDL_image.h>
#include <SDL_ttf.h>
#pragma warning (pop)

#include "Mage/ResourceManagement/Texture2D.h"
#include "Mage/ResourceManagement/Font.h"

void Mage::ResourceManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;

	// load support for png and jpg, this takes a while!

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		throw std::runtime_error(std::string("Failed to load support for png's: ") + SDL_GetError());
	}

	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG)
	{
		throw std::runtime_error(std::string("Failed to load support for jpg's: ") + SDL_GetError());
	}

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

std::shared_ptr<Mage::Texture2D> Mage::ResourceManager::LoadTexture(const std::string& file, float pixelsPerUnit, const glm::vec2& pivot) const
{
	const auto fullPath = m_DataPath + file;
	const auto pSurface = IMG_Load(fullPath.c_str());
	if (pSurface == nullptr)
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());

	// Finally create custom Texture2D
	auto texture = std::make_shared<Texture2D>(pSurface, pSurface->w, pSurface->h, pixelsPerUnit, pivot);

	SDL_FreeSurface(pSurface);

	return texture;
}

std::shared_ptr<Mage::Font> Mage::ResourceManager::LoadFont(const std::string& file, unsigned int size) const
{
	return std::make_shared<Font>(m_DataPath + file, size);
}