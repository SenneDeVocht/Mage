#include "Mage/MagePCH.h"
#include "TextComponent.h"

#include <SDL_ttf.h>
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "Mage/ResourceManagement/Font.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/SpriteComponent.h"
#include "Mage/ImGui/ImGuiHelper.h"
#include "Mage/ResourceManagement/Texture2D.h"

Mage::TextComponent::TextComponent(const std::string& text, const std::shared_ptr<Font>& font, const SDL_Color& color,
	float pixelsPerUnit, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignment)
	: m_NeedsUpdate{ true }
	, m_Text{ text }
	, m_Font{ font }
	, m_Color{ color }
	, m_HorizontalAlignment{ horizontalAlignment }
	, m_VerticalAlignment{ verticalAlignment }
	, m_PixelsPerUnit{ pixelsPerUnit }
{}

void Mage::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		// Create surface
		SDL_Surface* surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_Color);

		if (surf == nullptr)
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

		// Send to RenderComponent
		const auto pRendererComponent = m_pGameObject->GetComponentByType<SpriteComponent>();

		glm::vec2 pivot;
		switch (m_HorizontalAlignment)
		{
			case HorizontalAlignment::Left:
			{
				pivot.x = 0.f;
				break;
			}
			case HorizontalAlignment::Middle:
			{
				pivot.x = 0.5f;
				break;
			}
			case HorizontalAlignment::Right:
			{
				pivot.x = 1.f;
				break;
			}
		}
		switch (m_VerticalAlignment)
		{
			case VerticalAlignment::Top:
			{
				pivot.y = 1.f;
				break;
			}
			case VerticalAlignment::Middle:
			{
				pivot.y = 0.5f;
				break;
			}
			case VerticalAlignment::Bottom:
			{
				pivot.y = 0.f;
				break;
			}
		}

		if (pRendererComponent != nullptr)
			pRendererComponent->SetTexture(std::make_shared<Texture2D>(surf, surf->w, surf->h, m_PixelsPerUnit, pivot));

		// Free Surface
		SDL_FreeSurface(surf);

		// Reset flag
		m_NeedsUpdate = false;
	}
}

void Mage::TextComponent::DrawProperties()
{
	Mage::ImGuiHelper::Component("Text Component", this, &m_ShouldBeEnabled, [&]()
	{
		// Text
		m_NeedsUpdate |= ImGui::InputText("Text", &m_Text);

		// Color
		m_NeedsUpdate |= ImGuiHelper::SDLColorPicker("Color", &m_Color);

		// Horizontal Alignment
		m_NeedsUpdate |= ImGui::Combo("Horizontal Alignment", reinterpret_cast<int*>(&m_HorizontalAlignment), "Left\0Middle\0Right\0");

		// Vertical Alignment
		m_NeedsUpdate |= ImGui::Combo("Vertical Alignment", reinterpret_cast<int*>(&m_VerticalAlignment), "Top\0Middle\0Bottom\0");

		// Pixels per unit
		m_NeedsUpdate |= ImGui::DragFloat("Pixels Per Unit", &m_PixelsPerUnit, 0.1f, 0.f);
	});
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