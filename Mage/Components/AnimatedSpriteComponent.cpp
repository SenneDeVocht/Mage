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

Mage::AnimatedSpriteComponent::AnimatedSpriteComponent(std::shared_ptr<Texture2D> pSpritesheet, int numFrames, float secondsPerFrame, float layer)
	: m_Layer{ layer }
	, m_NumFrames{ numFrames }
    , m_SecondsPerFrame{ secondsPerFrame }
{
	SetSpritesheet(pSpritesheet);
}

void Mage::AnimatedSpriteComponent::Update()
{
	if (m_NumFrames < 1)
		return;


	m_Timer += Mage::Timer::GetInstance().GetDeltaTime();

	if (m_Timer >= m_SecondsPerFrame)
	{
		m_Timer -= m_SecondsPerFrame;

		// Next frame and loop
		++m_CurrentFrame;
		m_CurrentFrame %= m_NumFrames;
	}
}

void Mage::AnimatedSpriteComponent::DrawProperties()
{
	Mage::ImGuiHelper::Component("Animated Sprite Component", this, &m_ShouldBeEnabled, [&]()
	{
	    // Texture Image
	    ImGuiHelper::Texture(m_pSpritesheet.get());

		ImGuiHelper::ItemLabel("Number Of Frames", ImGuiHelper::ItemLabelAlignment::Left);
	    ImGui::DragInt("##Number Of Frames", &m_NumFrames);

		ImGuiHelper::ItemLabel("Seconds Per Frame", ImGuiHelper::ItemLabelAlignment::Left);
	    ImGui::DragFloat("##Seconds Per Frame", &m_SecondsPerFrame);

		ImGuiHelper::ItemLabel("Layer", ImGuiHelper::ItemLabelAlignment::Left);
		ImGui::DragFloat("##Layer", &m_Layer, 0.1f);
	});
}

void Mage::AnimatedSpriteComponent::Render() const
{
	if (m_pSpritesheet != nullptr)
	{
		// Get partial texture rect
		const int srcWidth = m_pSpritesheet->GetWidth() / m_NumFrames;
		const int srcHeight = m_pSpritesheet->GetHeight();
		const int srcX = m_CurrentFrame * srcWidth;

		// Get pos rot scale
		const auto& pos = m_pGameObject->GetTransform()->GetWorldPosition();
		const auto& rot = m_pGameObject->GetTransform()->GetWorldRotation();
		const auto& scale = m_pGameObject->GetTransform()->GetWorldScale();

		// Render
		ServiceLocator::GetRenderer()->RenderPartialTexture(m_pSpritesheet.get(), srcX, 0, srcWidth, srcHeight, pos, rot, scale, m_Layer);
	}
}

void Mage::AnimatedSpriteComponent::SetSpritesheet(std::shared_ptr<Texture2D> pTexture)
{
	m_pSpritesheet = pTexture;
}
