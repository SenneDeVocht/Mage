#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
	class Font;
	class Texture2D;

	class TextComponent : public Component
	{
	public:
		enum class HorizontalAlignment
		{
			Left,
			Middle,
			Right
		};
		enum class VerticalAlignment
		{
			Top,
			Middle,
			Bottom
		};

		explicit TextComponent(const std::string& text, const std::shared_ptr<Font>& font, const SDL_Color& color, float pixelsPerUnit,
			HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left, VerticalAlignment verticalAlignment = VerticalAlignment::Middle);

		void Update() override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);

	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
		SDL_Color m_Color;
		glm::vec2 m_Pivot;
		float m_PixelsPerUnit;
	};
}