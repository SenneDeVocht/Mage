#include "Mage/MagePCH.h"
#include "TextComponent.h"

#include <SDL_ttf.h>
#include "Mage/ResourceManagement/Font.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/SpriteComponent.h"
#include "Mage/Engine/Renderer.h"
#include "Mage/ResourceManagement/Texture2D.h"

Mage::TextComponent::TextComponent(const std::string& text, const std::shared_ptr<Font>& font, const SDL_Color& color,
	float pixelsPerUnit, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignment)
	: m_NeedsUpdate{ true }
	, m_Text{ text }
	, m_Font{ font }
	, m_Color{ color }
	, m_PixelsPerUnit{ pixelsPerUnit }
{
	switch (horizontalAlignment)
	{
		case HorizontalAlignment::Left:
		{
			m_Pivot.x = 0.f;
			break;
		}
		case HorizontalAlignment::Middle:
		{
			m_Pivot.x = 0.5f;
			break;
		}
		case HorizontalAlignment::Right:
		{
			m_Pivot.x = 1.f;
			break;
		}
	}

	switch (verticalAlignment)
	{
		case VerticalAlignment::Top:
		{
			m_Pivot.y = 1.f;
			break;
		}
		case VerticalAlignment::Middle:
		{
			m_Pivot.y = 0.5f;
			break;
		}
		case VerticalAlignment::Bottom:
		{
			m_Pivot.y = 0.f;
			break;
		}
	}
}

void Mage::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		// Create surface
		SDL_Surface* surf = TTF_RenderText_Solid(m_Font->GetFont(), m_Text.c_str(), m_Color);

		if (surf == nullptr)
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

		// Create Texture
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());

		// Free Surface
		SDL_FreeSurface(surf);

		// Send to RenderComponent
		const auto pRendererComponent = m_pGameObject->GetComponentByType<SpriteComponent>();

		if (pRendererComponent != nullptr)
			pRendererComponent->SetTexture(std::make_shared<Texture2D>(texture, m_PixelsPerUnit, m_Pivot));

		// Reset flag
		m_NeedsUpdate = false;
	}
}

void Mage::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void Mage::TextComponent::SetColor(const SDL_Color& color)
{
	m_Color = color;
	m_NeedsUpdate = true;
}