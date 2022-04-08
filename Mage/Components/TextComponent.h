#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
	class Font;
	class Texture2D;

	class TextComponent : public Component
	{
	public:
		explicit TextComponent(const std::string& text, const std::shared_ptr<Font>& font, const SDL_Color& color, bool renderBlended = true);

		void Update() override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);
		void SetRenderBlended(bool renderBlended);

	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
		SDL_Color m_Color;
		bool m_RenderBlended;
	};
}