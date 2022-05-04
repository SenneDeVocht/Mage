#include "Mage/MagePCH.h"
#include "TilemapComponent.h"

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/Transform.h"
#include "Mage/Engine/Renderer.h"
#include "Mage/Engine/ServiceLocator.h"

Mage::TilemapComponent::TilemapComponent(const std::vector<std::shared_ptr<Texture2D>>& tileTextures, const glm::vec2& tileSize)
    : m_TileTextures{ tileTextures }
    , m_TileSize{ tileSize }
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
        glm::vec2 position = glm::vec2{ tile.first.x, tile.first.y } * m_TileSize * wScale;
        position = glm::vec2{
            position.x * cos(rotInRad) - position.y * sin(rotInRad),
            position.x * sin(rotInRad) + position.y * cos(rotInRad)
        };

        position += wPos;

        // Render
        ServiceLocator::GetRenderer()->RenderTexture(*m_TileTextures[tile.second], position, wRot, wScale);
    }
}

void Mage::TilemapComponent::SetTile(const glm::ivec2& position, int tileIndex)
{
    if (tileIndex >= m_TileTextures.size())
        std::cout << "Tile index out of range" << std::endl;

    else if (tileIndex < 0)
        m_Tilemap.erase(position);
    
    else
        m_Tilemap[position] = tileIndex;
}
