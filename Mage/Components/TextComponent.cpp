#include "Mage/MagePCH.h"
#include "TextComponent.h"

#pragma warning(push, 0)
#include <IconsFontAwesome.h>
#include <SDL_ttf.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"
#pragma warning(pop)

#include "Mage/ResourceManagement/Font.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/SpriteComponent.h"
#include "Mage/Engine/Renderer.h"
#include "Mage/ImGui/ImGuiHelper.h"
#include "Mage/ResourceManagement/Texture2D.h"
#include "Mage/Components/Transform.h"
#include "Mage/Engine/ServiceLocator.h"

Mage::TextComponent::TextComponent(const std::string& text, const std::shared_ptr<Font>& font, const SDL_Color& color, float pixelsPerUnit,
                                   const glm::vec2& pivot, TextAlignment alignment, int lineSpacing, float layer)
    : m_Layer{ layer }
    , m_Font{ font }
	, m_Text{ text }
	, m_Color{ color }
	, m_Alignment{ alignment }
    , m_LineSpacing{ lineSpacing }
	, m_PixelsPerUnit{ pixelsPerUnit }
    , m_Pivot{ pivot }
{}

void Mage::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		// Split text in lines
		std::vector<std::string> lines;

		size_t last = 0;
	    size_t next;
	    while ((next = m_Text.find('\n', last)) != std::string::npos)
	    {
            lines.push_back(m_Text.substr(last, next - last));
	        last = next + 1;
	    }
		lines.push_back(m_Text.substr(last));

		// Get final surface dimensions
		int width = 0;
		int height = 0;

		for (auto& string : lines)
		{
			int linew, lineh;
    		TTF_SizeText(m_Font->GetFont(), string.c_str(), &linew, &lineh);

			width = std::max(width, linew);
			height += lineh;
		}

		height += (static_cast<int>(lines.size()) - 1) * m_LineSpacing;

		// Create final surface
		SDL_Surface* finalSurf = SDL_CreateRGBSurface(0, width, height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

		// Render lines
		for (int i = 0; i < (int)lines.size(); i++)
		{
			SDL_Surface* lineSurf = TTF_RenderText_Blended(m_Font->GetFont(), lines[i].c_str(), m_Color);

			if (lineSurf != nullptr)
			{
				SDL_Rect dstRect{};
				dstRect.y = i * (lineSurf->h + m_LineSpacing);
				dstRect.w = lineSurf->w;
				dstRect.h = lineSurf->h;

                switch (m_Alignment)
                {
                    case TextAlignment::Left:
                        dstRect.x = 0;
                        break;
					case TextAlignment::Center:
                        dstRect.x = (width - lineSurf->w) / 2;
					    break;
					case TextAlignment::Right:
                        dstRect.x = width - lineSurf->w;
					    break;
                }

				SDL_BlitSurface(lineSurf, nullptr, finalSurf, &dstRect);

				// Free Surface
				SDL_FreeSurface(lineSurf);
			}
		}

		// Send to texture
		m_pTexture = std::make_unique<Texture2D>(finalSurf, width, height, m_PixelsPerUnit, m_Pivot);

		// Free surface
		SDL_FreeSurface(finalSurf);

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

		ServiceLocator::GetRenderer()->RenderTexture(*m_pTexture, pos, rot, scale, m_Layer);
	}
}

void Mage::TextComponent::DrawProperties()
{
	Mage::ImGuiHelper::Component("Text Component", this, &m_ShouldBeEnabled, [&]()
	{
		// Texture Image
        if (!m_Text.empty())
			ImGuiHelper::Texture(*m_pTexture);

		// Text
	    ImGuiHelper::ItemLabel("Text", ImGuiHelper::ItemLabelAlignment::Left);
		m_NeedsUpdate |= ImGui::InputTextMultiline("##Text", &m_Text);

		// Color
		ImGuiHelper::ItemLabel("Color", ImGuiHelper::ItemLabelAlignment::Left);
		m_NeedsUpdate |= ImGuiHelper::SDLColorPicker("##Color", &m_Color);

		// Alignment
		int activeButton = static_cast<int>(m_Alignment);

		ImGuiHelper::ItemLabel("Alignment", ImGuiHelper::ItemLabelAlignment::Left);
		float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

		m_NeedsUpdate |= ImGuiHelper::RadioToggleButton(ICON_FA_ALIGN_LEFT, &activeButton, 0, { ImGui::CalcItemWidth(), 0.f });
		ImGui::SameLine(0, spacing);
		ImGui::PopItemWidth();

		m_NeedsUpdate |= ImGuiHelper::RadioToggleButton(ICON_FA_ALIGN_CENTER, &activeButton, 1, { ImGui::CalcItemWidth(), 0.f });
		ImGui::SameLine(0, spacing);
		ImGui::PopItemWidth();

		m_NeedsUpdate |= ImGuiHelper::RadioToggleButton(ICON_FA_ALIGN_RIGHT, &activeButton, 2, { ImGui::CalcItemWidth(), 0.f });
		ImGui::PopItemWidth();

		m_Alignment = static_cast<TextAlignment>(activeButton);

		// Line Spacing
		ImGuiHelper::ItemLabel("Line Spacing", ImGuiHelper::ItemLabelAlignment::Left);
		m_NeedsUpdate |= ImGui::DragInt("##Line Spacing", &m_LineSpacing, 0.1f);

		// Pixels per unit
		ImGuiHelper::ItemLabel("Pixels Per Unit", ImGuiHelper::ItemLabelAlignment::Left);
		m_NeedsUpdate |= ImGui::DragFloat("##Pixels Per Unit", &m_PixelsPerUnit, 0.1f, 0.f);

		// Pivot
        ImGuiHelper::ItemLabel("Pivot", ImGuiHelper::ItemLabelAlignment::Left);
		m_NeedsUpdate |= ImGui::DragFloat2("##Pivot", &m_Pivot.x, 0.1f, 0.f, 1.f);

		// Layer
		ImGuiHelper::ItemLabel("Layer", ImGuiHelper::ItemLabelAlignment::Left);
		ImGui::DragFloat("##Layer", &m_Layer, 0.1f);
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