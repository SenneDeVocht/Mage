#include "Mage/MagePCH.h"
#include "AnimatedSpriteComponent.h"

#include "imgui.h"

#include "Mage/Engine/ServiceLocator.h"
#include "Mage/Engine/Renderer.h"
#include "Mage/Engine/Timer.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/Transform.h"
#include "Mage/ImGui/ImGuiHelper.h"
#include "Mage/ResourceManagement/Texture2D.h"

Mage::AnimatedSpriteComponent::AnimatedSpriteComponent(float renderLayer)
	: m_RenderLayer{ renderLayer }
{}

void Mage::AnimatedSpriteComponent::Update()
{
	if (m_pAnimation == nullptr)
		return;

	if (m_pAnimation->NumFrames < 1)
		return;

	if (m_CurrentFrame == m_pAnimation->NumFrames - 1 && !m_pAnimation->Loop)
		return;

	m_Timer += Timer::GetInstance().GetDeltaTime();

	if (m_Timer >= m_pAnimation->SecondsPerFrame)
	{
		m_Timer = 0;

		// Next frame and loop
		++m_CurrentFrame;
		m_CurrentFrame %= m_pAnimation->NumFrames;
	}
}

void Mage::AnimatedSpriteComponent::DrawProperties()
{
	Mage::ImGuiHelper::Component("Animated Sprite Component", this, &m_ShouldBeEnabled, [&]()
	{
	    // Texture Image
	    ImGuiHelper::Texture(m_pAnimation->pSpritesheet.get());
		
		ImGuiHelper::ItemLabel("Layer", ImGuiHelper::ItemLabelAlignment::Left);
		ImGui::DragFloat("##Layer", &m_RenderLayer, 0.1f);
	});
}

void Mage::AnimatedSpriteComponent::Render() const
{
	if (m_pAnimation != nullptr && m_pAnimation->pSpritesheet != nullptr && m_pAnimation->NumFrames > 0)
	{
		// Get partial texture rect
		const int srcWidth = m_pAnimation->pSpritesheet->GetWidth() / m_pAnimation->NumFrames;
		const int srcHeight = m_pAnimation->pSpritesheet->GetHeight();
		const int srcX = m_CurrentFrame * srcWidth;

		// Get pos rot scale
		const auto& pos = m_pGameObject->GetTransform()->GetWorldPosition();
		const auto& rot = m_pGameObject->GetTransform()->GetWorldRotation();
		const auto& scale = m_pGameObject->GetTransform()->GetWorldScale();

		// Render
		ServiceLocator::GetRenderer()->RenderPartialTexture(m_pAnimation->pSpritesheet.get(), srcX, 0, srcWidth, srcHeight, pos, rot, scale, m_RenderLayer);
	}
}

void Mage::AnimatedSpriteComponent::SetAnimation(const std::shared_ptr<SpriteAnimation>& animation)
{
	m_pAnimation = animation;

	m_CurrentFrame = 0;
	m_Timer = 0;
}