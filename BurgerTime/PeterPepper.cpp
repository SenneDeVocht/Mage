#include "BurgerTime/BurgerTimePCH.h"
#include "PeterPepper.h"

#include "Level.h"
#include "Mage/Input/InputManager.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/Transform.h"
#include "Mage/Engine/Timer.h"
#include "Mage/Components/AnimatedSpriteComponent.h"
#include "Mage/Components/RigidBodyComponent.h"

PeterPepper::PeterPepper(Level* level, Mage::AnimatedSpriteComponent* pIdle, Mage::AnimatedSpriteComponent* pWalkfront,
                         Mage::AnimatedSpriteComponent* pWalkBack, Mage::AnimatedSpriteComponent* pWalkLeft, Mage::AnimatedSpriteComponent* pWalkRight)
    : m_pLevel{ level }
	, m_pIdle{ pIdle }
	, m_pWalkFront{ pWalkfront }
	, m_pWalkBack{ pWalkBack }
	, m_pWalkLeft{ pWalkLeft }
	, m_pWalkRight{ pWalkRight }
{}

void PeterPepper::Initialize()
{
	m_pRigidBody = GetGameObject()->GetComponentByType<Mage::RigidBodyComponent>();
}


void PeterPepper::Update()
{
	// INPUT
	//------
	m_Input = { 0, 0 };

	if (Mage::InputManager::GetInstance().CheckKeyboardKey(0x26, Mage::InputState::Hold))
		++m_Input.y;
	if (Mage::InputManager::GetInstance().CheckKeyboardKey(0x28, Mage::InputState::Hold))
		--m_Input.y;

	if (m_Input.y == 0)
	{
		if (Mage::InputManager::GetInstance().CheckKeyboardKey(0x25, Mage::InputState::Hold))
			--m_Input.x;
		if (Mage::InputManager::GetInstance().CheckKeyboardKey(0x27, Mage::InputState::Hold))
			++m_Input.x;
	}

	// ANIMATIONS
	//-----------
	m_pIdle->SetEnabled(false);
	m_pWalkFront->SetEnabled(false);
	m_pWalkBack->SetEnabled(false);
	m_pWalkLeft->SetEnabled(false);
	m_pWalkRight->SetEnabled(false);

	if (m_Input.x == 0 && m_Input.y == 0)
	{
		m_pIdle->SetEnabled(true);
	}
	else if (m_Input.x > 0)
	{
		m_pWalkRight->SetEnabled(true);
	}
	else if (m_Input.x < 0)
	{
		m_pWalkLeft->SetEnabled(true);
	}
	else if (m_Input.y > 0)
	{
		m_pWalkBack->SetEnabled(true);
	}
	else if (m_Input.y < 0)
	{
		m_pWalkFront->SetEnabled(true);
	}
}

void PeterPepper::FixedUpdate()
{
	// MOVE
	//-----
	const glm::vec2 position = m_pGameObject->GetTransform()->GetWorldPosition();
	glm::vec2 velocity{ 0, 0 };

	// Up - Down
	if (m_Input.y > 0 && m_pLevel->CanMoveInDirection(position, Level::Direction::Up) ||
		m_Input.y < 0 && m_pLevel->CanMoveInDirection(position, Level::Direction::Down))
	{
		velocity.y = m_Input.y * m_Speed;
	}

	// Left - Right
	if (m_Input.x < 0 && m_pLevel->CanMoveInDirection(position, Level::Direction::Left) ||
		m_Input.x > 0 && m_pLevel->CanMoveInDirection(position, Level::Direction::Right))
	{
		velocity.x = m_Input.x * m_Speed;
		GetGameObject()->GetTransform()->SetPosition(m_pLevel->SnapToPlatform(position));
	}

	m_pRigidBody->SetVelocity(velocity);
}
