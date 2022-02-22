#include "MagePCH.h"
#include "TextComponent.h"
#include <SDL_ttf.h>
#include "Font.h"
#include "GameObject.h"
#include "RendererComponent.h"
#include "Renderer.h"
#include "Texture2D.h"

TextComponent::TextComponent(const std::string& text, const std::shared_ptr<Font>& font, const SDL_Color& color, bool renderBlended)
	: m_NeedsUpdate{ true }
	, m_Text{ text }
	, m_Font{ font }
	, m_Color{ color }
	, m_RenderBlended{ renderBlended }
{
}

void TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		// Create surface
		SDL_Surface* surf;

        if (m_RenderBlended)
			surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_Color);
        else
            surf = TTF_RenderText_Solid(m_Font->GetFont(), m_Text.c_str(), m_Color);

		if (surf == nullptr)
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

		// Create Texture
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);

		if (texture == nullptr)
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());

		// Free Surface
		SDL_FreeSurface(surf);

		// Send to RenderComponent
		auto pRendererComponent = m_pGameObject->GetComponentByType<RendererComponent>();

		if (pRendererComponent != nullptr)
			pRendererComponent->SetTexture(std::make_shared<Texture2D>(texture));

		// Reset flag
		m_NeedsUpdate = false;
	}
}

void TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void TextComponent::SetColor(const SDL_Color& color)
{
	m_Color = color;
	m_NeedsUpdate = true;
}