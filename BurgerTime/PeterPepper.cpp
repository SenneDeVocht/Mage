#include "BurgerTime/BurgerTimePCH.h"
#include "PeterPepper.h"

#include "Mage/Input/InputManager.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/Transform.h"
#include "Mage/Engine/Timer.h"
#include "Mage/Components/AnimatedSpriteComponent.h"
#include "Mage/Components/RigidBodyComponent.h"
#include "Mage/Components/BoxColliderComponent.h"

PeterPepper::PeterPepper(Mage::AnimatedSpriteComponent* pIdle, Mage::AnimatedSpriteComponent* pWalkfront,
                         Mage::AnimatedSpriteComponent* pWalkBack, Mage::AnimatedSpriteComponent* pWalkLeft, Mage::AnimatedSpriteComponent* pWalkRight)
    : m_pIdle{ pIdle }
	, m_pWalkFront{ pWalkfront }
	, m_pWalkBack{ pWalkBack }
	, m_pWalkLeft{ pWalkLeft }
	, m_pWalkRight{ pWalkRight }
{}

void PeterPepper::Initialize()
{
	m_pRigidbody = m_pGameObject->GetComponentByType<Mage::RigidBodyComponent>();
}

void PeterPepper::OnTriggerEnter(Mage::BoxColliderComponent* other)
{
	if (other->GetGameObject()->GetTag() == "Platform")
	{
		++m_NumPlatformsTouching;
		m_pLastPlatformTouched = other;
	}

	if (other->GetGameObject()->GetTag() == "Ladder")
		++m_NumLaddersTouching;
}

void PeterPepper::OnTriggerExit(Mage::BoxColliderComponent* other)
{
	if (other->GetGameObject()->GetTag() == "Platform")
		--m_NumPlatformsTouching;

	if (other->GetGameObject()->GetTag() == "Ladder")
		--m_NumLaddersTouching;
}

void PeterPepper::Update()
{
	// Input
	m_InputDir = { 0, 0 };

	if (Mage::InputManager::GetInstance().CheckKeyboardKey(0x26, Mage::InputState::Hold))
		++m_InputDir.y;
	if (Mage::InputManager::GetInstance().CheckKeyboardKey(0x28, Mage::InputState::Hold))
		--m_InputDir.y;

	if (m_InputDir.y == 0)
	{
		if (Mage::InputManager::GetInstance().CheckKeyboardKey(0x25, Mage::InputState::Hold))
			--m_InputDir.x;
		if (Mage::InputManager::GetInstance().CheckKeyboardKey(0x27, Mage::InputState::Hold))
			++m_InputDir.x;
	}

	// Animations
	m_pIdle->SetEnabled(false);
	m_pWalkFront->SetEnabled(false);
	m_pWalkBack->SetEnabled(false);
	m_pWalkLeft->SetEnabled(false);
	m_pWalkRight->SetEnabled(false);

	if (m_InputDir.x == 0 && m_InputDir.y == 0)
	{
		m_pIdle->SetEnabled(true);
	}
	else if (m_InputDir.x > 0)
	{
		m_pWalkRight->SetEnabled(true);
	}
	else if (m_InputDir.x < 0)
	{
		m_pWalkLeft->SetEnabled(true);
	}
	else if (m_InputDir.y > 0)
	{
		m_pWalkBack->SetEnabled(true);
	}
	else if (m_InputDir.y < 0)
	{
		m_pWalkFront->SetEnabled(true);
	}
}

void PeterPepper::FixedUpdate()
{
	glm::vec2 velocity{ 0, 0 };

	// Can only move horizontally if touching platform
	if (m_NumPlatformsTouching > 0)
		velocity.x = m_InputDir.x * m_Speed;

	// Can only move vertically if touching ladder
	if (m_NumLaddersTouching > 0)
		velocity.y = m_InputDir.y * m_Speed;


	GetGameObject()->GetComponentByType<Mage::RigidBodyComponent>()->SetVelocity(velocity);

	// If not touching ladder, but touching platform, stick to platform
	if (m_NumPlatformsTouching > 0 && m_NumLaddersTouching == 0)
	{
		const float platformHeight = m_pLastPlatformTouched->GetGameObject()->GetTransform()->GetWorldPosition().y;
		constexpr float offset = 0.4375f;

		const glm::vec2 position = { m_pGameObject->GetTransform()->GetWorldPosition().x, platformHeight + offset };

		if (abs(m_pGameObject->GetTransform()->GetWorldPosition().y - position.y) > 0.0001f)
			m_pRigidbody->SetPosition(position);
	}
}
