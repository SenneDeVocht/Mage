#include "Mage/MagePCH.h"
#include "SpriteComponent.h"

#include "Mage/Engine/Renderer.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/ResourceManagement/ResourceManager.h"
#include "Mage/Components/Transform.h"

Mage::SpriteComponent::SpriteComponent(const std::string& filename)
{
	SetTexture(filename);
}

void Mage::SpriteComponent::Render() const
{
	if (m_pTexture != nullptr)
	{
		const auto& pos = m_pGameObject->GetTransform()->GetWorldPosition();

		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
	}
}

void Mage::SpriteComponent::SetTexture(std::shared_ptr<Texture2D> pTexture)
{
	m_pTexture = pTexture;
}

void Mage::SpriteComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}
