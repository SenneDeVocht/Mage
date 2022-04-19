#include "Mage/MagePCH.h"
#include "SpriteComponent.h"

#include "imgui.h"

#include "Mage/Engine/Renderer.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/ResourceManagement/ResourceManager.h"
#include "Mage/Components/Transform.h"
#include "Mage/ImGui/ImGuiHelper.h"
#include "Mage/ResourceManagement/Texture2D.h"

Mage::SpriteComponent::SpriteComponent(std::shared_ptr<Texture2D> pTexture)
{
	SetTexture(pTexture);
}

void Mage::SpriteComponent::DrawProperties()
{
	Mage::ImGuiHelper::Component("Sprite Component", this, &m_ShouldBeEnabled, [&]()
	{
		// Texture Image
		float availableWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
		float availableHeight = 50.f;

		float scaleFactorX = availableWidth / m_pTexture->GetWidth();
		float scaleFactorY = availableHeight / m_pTexture->GetHeight();

		float scaleFactor = std::min(scaleFactorX, scaleFactorY);

		ImGui::Image((void*)(intptr_t)m_pTexture->GetGLTexture(), { m_pTexture->GetWidth() * scaleFactor, m_pTexture->GetHeight() * scaleFactor });
	});
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