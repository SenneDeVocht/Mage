#include "BurgerTime/BurgerTimePCH.h"
#include "PeterPepper.h"

#include "Mage/Input/InputManager.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/Transform.h"
#include "Mage/Engine/Timer.h"
#include "Mage/Components/AnimatedSpriteComponent.h"

PeterPepper::PeterPepper(Mage::AnimatedSpriteComponent* pIdle, Mage::AnimatedSpriteComponent* pWalkfront,
	Mage::AnimatedSpriteComponent* pWalkBack, Mage::AnimatedSpriteComponent* pWalkLeft, Mage::AnimatedSpriteComponent* pWalkRight)
    : m_pIdle{ pIdle }
	, m_pWalkFront{ pWalkfront }
	, m_pWalkBack{ pWalkBack }
	, m_pWalkLeft{ pWalkLeft }
	, m_pWalkRight{ pWalkRight }
{}

void PeterPepper::Update()
{
	// Determine input direction
	int horizontalDir{ 0 };
	int verticalDir{ 0 };

	if (Mage::InputManager::GetInstance().CheckKeyboardKey(0x26, Mage::InputState::Hold))
		++verticalDir;
	if (Mage::InputManager::GetInstance().CheckKeyboardKey(0x28, Mage::InputState::Hold))
		--verticalDir;
	if (Mage::InputManager::GetInstance().CheckKeyboardKey(0x25, Mage::InputState::Hold))
		--horizontalDir;
	if (Mage::InputManager::GetInstance().CheckKeyboardKey(0x27, Mage::InputState::Hold))
		++horizontalDir;

	// Move
	const glm::vec2 movement = glm::vec2(horizontalDir, verticalDir) * Mage::Timer::GetInstance().GetDeltaTime() * m_Speed;
	this->GetGameObject()->GetTransform()->Translate(movement.x, movement.y);

	// Animations
	m_pIdle->SetEnabled(false);
	m_pWalkFront->SetEnabled(false);
	m_pWalkBack->SetEnabled(false);
	m_pWalkLeft->SetEnabled(false);
	m_pWalkRight->SetEnabled(false);

	if (horizontalDir == 0 && verticalDir == 0)
	{
		m_pIdle->SetEnabled(true);
	}
	else if (horizontalDir > 0)
	{
		m_pWalkRight->SetEnabled(true);
	}
	else if (horizontalDir < 0)
	{
		m_pWalkLeft->SetEnabled(true);
	}
	else if (verticalDir > 0)
	{
		m_pWalkBack->SetEnabled(true);
	}
	else if (verticalDir < 0)
	{
		m_pWalkFront->SetEnabled(true);
	}
}
