#include "Mage/MagePCH.h"
#include "TextComponent.h"

#include <SDL_ttf.h>
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "Mage/ResourceManagement/Font.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/SpriteComponent.h"
#include "Mage/Engine/Renderer.h"
#include "Mage/ImGui/ImGuiHelper.h"
#include "Mage/ResourceManagement/Texture2D.h"
#include "Mage/Components/Transform.h"

Mage::TextComponent::TextComponent(const std::string& text, const std::shared_ptr<Font>& font, const SDL_Color& color, float pixelsPerUnit,
	const glm::vec2& pivot, TextAlignment alignment)
    : m_Text{ text }
	, m_Font{ font }
	, m_Color{ color }
	, m_PixelsPerUnit{ pixelsPerUnit }
    , m_Pivot{ pivot }
	, m_Alignment{ alignment }
{}

void Mage::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		// Create surface
		SDL_Surface* surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_Color);

		if (surf != nullptr)
		{
			// Send to Texture
			m_pTexture = std::make_unique<Texture2D>(surf, surf->w, surf->h, m_PixelsPerUnit, m_Pivot);

			// Free Surface
			SDL_FreeSurface(surf);
		}

		// Reset flag
		m_NeedsUpdate = false;
	}
}

void Mage::TextComponent::Render() const
{
	if (m_pTexture != nullptr && !m_Text.empty())
	{
		const auto& pos = m_pGameObject->GetTransform()->GetWorldPosition();
		const auto& rot = m_pGameObject->GetTransform()->GetWorldRotation();
		const auto& scale = m_pGameObject->GetTransform()->GetWorldScale();

		Renderer::GetInstance().RenderTexture(*m_pTexture, pos, rot, scale);
	}
}

void Mage::TextComponent::DrawProperties()
{
	Mage::ImGuiHelper::Component("Text Component", this, &m_ShouldBeEnabled, [&]()
	{
		// Texture Image
        if (!m_Text.empty())
        {
			float availableWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
			float availableHeight = 50.f;

			float scaleFactorX = availableWidth / m_pTexture->GetWidth();
			float scaleFactorY = availableHeight / m_pTexture->GetHeight();

			float scaleFactor = std::min(scaleFactorX, scaleFactorY);

			ImGui::Image((void*)(intptr_t)m_pTexture->GetGLTexture(), { m_pTexture->GetWidth() * scaleFactor, m_pTexture->GetHeight() * scaleFactor });
        }

		// Text
	    ImGuiHelper::ItemLabel("Text", ImGuiHelper::ItemLabelAlignment::Left);
		m_NeedsUpdate |= ImGui::InputText("##Text", &m_Text);

		// Color
		ImGuiHelper::ItemLabel("Color", ImGuiHelper::ItemLabelAlignment::Left);
		m_NeedsUpdate |= ImGuiHelper::SDLColorPicker("##Color", &m_Color);

		// Alignment
		ImGuiHelper::ItemLabel("Alignment", ImGuiHelper::ItemLabelAlignment::Left);
		m_NeedsUpdate |= ImGui::Combo("##Alignment", reinterpret_cast<int*>(&m_Alignment), "Left\0Middle\0Right\0");
		
		// Pixels per unit
		ImGuiHelper::ItemLabel("Pixels Per Unit", ImGuiHelper::ItemLabelAlignment::Left);
		m_NeedsUpdate |= ImGui::DragFloat("##Pixels Per Unit", &m_PixelsPerUnit, 0.1f, 0.f);
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