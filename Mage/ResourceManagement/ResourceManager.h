#pragma once
#include "Mage/Singleton.h"

namespace Mage
{
	class Texture2D;
	class Font;
	class SoundClip;

	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		std::shared_ptr<Texture2D> LoadTexture(const std::string& file, float pixelsPerUnit, const glm::vec2& pivot = { 0.5f, 0.5f }) const;
		std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size) const;
		std::shared_ptr<SoundClip> LoadSoundClip(const std::string& file) const;

	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_DataPath;
	};
}