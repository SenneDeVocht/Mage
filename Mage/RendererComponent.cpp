#include "MagePCH.h"
#include "RendererComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Transform.h"

RendererComponent::RendererComponent(const std::string& filename)
{
	SetTexture(filename);
}

void RendererComponent::Render() const
{
	if (m_pTexture != nullptr)
	{
		const auto& pos = m_pGameObject->GetTransform()->GetWorldPosition();

		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
	}
}

void RendererComponent::SetTexture(std::shared_ptr<Texture2D> pTexture)
{
	m_pTexture = pTexture;
}

void RendererComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}