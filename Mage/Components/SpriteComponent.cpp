#include "Mage/MagePCH.h"
#include "SpriteComponent.h"

#include "imgui.h"

#include "Mage/Engine/ServiceLocator.h"
#include "Mage/Engine/Renderer.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/Transform.h"
#include "Mage/ImGui/ImGuiHelper.h"
#include "Mage/ResourceManagement/Texture2D.h"

Mage::SpriteComponent::SpriteComponent(float layer)
	: m_Layer{ layer }
{}

Mage::SpriteComponent::SpriteComponent(std::shared_ptr<Texture2D> pTexture, float layer)
    : m_Layer{ layer }
{
	SetTexture(pTexture);
}

void Mage::SpriteComponent::DrawProperties()
{
	Mage::ImGuiHelper::Component("Sprite Component", this, &m_ShouldBeEnabled, [&]()
	{
		// Texture Image
	    ImGuiHelper::Texture(*m_pTexture);

		ImGuiHelper::ItemLabel("Layer", ImGuiHelper::ItemLabelAlignment::Left);
		ImGui::DragFloat("##Layer", &m_Layer, 0.1f);
	});
}

void Mage::SpriteComponent::Render() const
{
	if (m_pTexture != nullptr)
	{
		const auto& pos = m_pGameObject->GetTransform()->GetWorldPosition();
		const auto& rot = m_pGameObject->GetTransform()->GetWorldRotation();
		const auto& scale = m_pGameObject->GetTransform()->GetWorldScale();

		ServiceLocator::GetRenderer()->RenderTexture(*m_pTexture, pos, rot, scale, m_Layer);
	}
}

void Mage::SpriteComponent::SetTexture(std::shared_ptr<Texture2D> pTexture)
{
	m_pTexture = pTexture;
}