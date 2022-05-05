#include "Mage/MagePCH.h"
#include "TilemapComponent.h"

#include "imgui.h"
#include "Mage/ImGui/ImGuiHelper.h"

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/Transform.h"
#include "Mage/Engine/Renderer.h"
#include "Mage/Engine/ServiceLocator.h"

Mage::TilemapComponent::TilemapComponent(const std::vector<std::shared_ptr<Texture2D>>& tileTextures, const glm::vec2& tileSpacing)
    : m_TileTextures{ tileTextures }
    , m_TileSpacing{ tileSpacing }
{}

void Mage::TilemapComponent::Render() const
{
    const auto wPos = GetGameObject()->GetTransform()->GetWorldPosition();
    const auto wRot = GetGameObject()->GetTransform()->GetWorldRotation();
    const auto wScale = GetGameObject()->GetTransform()->GetWorldScale();

    const auto rotInRad = glm::radians(wRot);

    for (const auto& tile : m_Tilemap)
    {
        // Position in world space
        glm::vec2 position = glm::vec2{ tile.first.x, tile.first.y } * m_TileSpacing * wScale;
        position = glm::vec2{
            position.x * cos(rotInRad) - position.y * sin(rotInRad),
            position.x * sin(rotInRad) + position.y * cos(rotInRad)
        };

        position += wPos;

        // Render
        ServiceLocator::GetRenderer()->RenderTexture(*m_TileTextures[tile.second], position, wRot, wScale);
    }
}

void Mage::TilemapComponent::DrawProperties()
{
    Mage::ImGuiHelper::Component("Tilemap Component", this, &m_ShouldBeEnabled, [&]()
    {
        // spacing
        ImGuiHelper::ItemLabel("Spacing", ImGuiHelper::ItemLabelAlignment::Left);
        ImGui::DragFloat2("##Spacing", &m_TileSpacing.x, 0.1f);

        for (const auto& texture : m_TileTextures)
        {
            ImGuiHelper::Texture(*texture);
        }
    });
}

void Mage::TilemapComponent::SetTile(const glm::ivec2& position, int tileIndex)
{
    if (tileIndex >= (int)m_TileTextures.size() || tileIndex < 0)
        std::cout << "Tile index out of range" << std::endl;
    
    else
        m_Tilemap[position] = tileIndex;
}

void Mage::TilemapComponent::EraseTile(const glm::ivec2& position)
{
    m_Tilemap.erase(position);
}
