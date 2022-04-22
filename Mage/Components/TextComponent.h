#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
	class Font;
	class Texture2D;

	class TextComponent : public Component
	{
	public:
		enum class TextAlignment
		{
			Left,
			Middle,
			Right
		};

		explicit TextComponent(const std::string& text, const std::shared_ptr<Font>& font, const SDL_Color& color, float pixelsPerUnit,
			const glm::vec2& pivot = { 0.0f, 0.0f }, TextAlignment alignment = TextAlignment::Left);

		void Update() override;
		void Render() const override;
		void DrawProperties() override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);

	private:
		std::unique_ptr<Texture2D> m_pTexture;

		bool m_NeedsUpdate{ true };
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
		SDL_Color m_Color;
		float m_PixelsPerUnit;
		glm::vec2 m_Pivot;
		TextAlignment m_Alignment;
	};
}