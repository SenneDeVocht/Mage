#include "Mage/MagePCH.h"
#include "AnimatedSpriteComponent.h"

#include "Mage/Engine/Renderer.h"
#include "Mage/Engine/Timer.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/ResourceManagement/ResourceManager.h"
#include "Mage/Components/Transform.h"
#include "Mage/ResourceManagement/Texture2D.h"

Mage::AnimatedSpriteComponent::AnimatedSpriteComponent(const std::string& filename, int numFrames, float secondsPerFrame)
	: m_NumFrames{ numFrames }
	, m_SecondsPerFrame{ secondsPerFrame }
{
	SetSpritesheet(filename);
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

void Mage::AnimatedSpriteComponent::Render() const
{
	if (m_pSpritesheet != nullptr)
	{
		// Get partial texture rect
		int srcWidth, srcHeight;
		SDL_QueryTexture(m_pSpritesheet->GetSDLTexture(), nullptr, nullptr, &srcWidth, &srcHeight);
		srcWidth /= m_NumFrames;
		int srcX = m_CurrentFrame * srcWidth;

		// Get screen pos
		const auto& pos = m_pGameObject->GetTransform()->GetWorldPosition();

		// Render
		Renderer::GetInstance().RenderPartialTexture(*m_pSpritesheet, srcX, 0, srcWidth, srcHeight, pos.x, pos.y);
	}
}

void Mage::AnimatedSpriteComponent::SetSpritesheet(std::shared_ptr<Texture2D> pTexture)
{
	m_pSpritesheet = pTexture;
}

void Mage::AnimatedSpriteComponent::SetSpritesheet(const std::string& filename)
{
	m_pSpritesheet = ResourceManager::GetInstance().LoadTexture(filename);
}
