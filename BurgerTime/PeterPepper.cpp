#include "BurgerTime/BurgerTimePCH.h"
#include "PeterPepper.h"

#include "Level.h"
#include "Mage/Input/InputManager.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/Transform.h"
#include "Mage/Engine/Timer.h"
#include "Mage/Components/AnimatedSpriteComponent.h"
#include "Mage/Components/RigidBodyComponent.h"
#include "Mage/Components/BoxColliderComponent.h"

PeterPepper::PeterPepper(Level* level, Mage::AnimatedSpriteComponent* pIdle, Mage::AnimatedSpriteComponent* pWalkfront,
                         Mage::AnimatedSpriteComponent* pWalkBack, Mage::AnimatedSpriteComponent* pWalkLeft, Mage::AnimatedSpriteComponent* pWalkRight)
    : m_pLevel{ level }
	, m_pIdle{ pIdle }
	, m_pWalkFront{ pWalkfront }
	, m_pWalkBack{ pWalkBack }
	, m_pWalkLeft{ pWalkLeft }
	, m_pWalkRight{ pWalkRight }
{}

void PeterPepper::Update()
{
	// INPUT
	//------
	glm::vec2 inputDir = { 0, 0 };

	if (Mage::InputManager::GetInstance().CheckKeyboardKey(0x26, Mage::InputState::Hold))
		++inputDir.y;
	if (Mage::InputManager::GetInstance().CheckKeyboardKey(0x28, Mage::InputState::Hold))
		--inputDir.y;

	if (inputDir.y == 0)
	{
		if (Mage::InputManager::GetInstance().CheckKeyboardKey(0x25, Mage::InputState::Hold))
			--inputDir.x;
		if (Mage::InputManager::GetInstance().CheckKeyboardKey(0x27, Mage::InputState::Hold))
			++inputDir.x;
	}

	// ANIMATIONS
	//-----------
	m_pIdle->SetEnabled(false);
	m_pWalkFront->SetEnabled(false);
	m_pWalkBack->SetEnabled(false);
	m_pWalkLeft->SetEnabled(false);
	m_pWalkRight->SetEnabled(false);

	if (inputDir.x == 0 && inputDir.y == 0)
	{
		m_pIdle->SetEnabled(true);
	}
	else if (inputDir.x > 0)
	{
		m_pWalkRight->SetEnabled(true);
	}
	else if (inputDir.x < 0)
	{
		m_pWalkLeft->SetEnabled(true);
	}
	else if (inputDir.y > 0)
	{
		m_pWalkBack->SetEnabled(true);
	}
	else if (inputDir.y < 0)
	{
		m_pWalkFront->SetEnabled(true);
	}


	// MOVE
	//-----
	const glm::vec2 position = m_pGameObject->GetTransform()->GetWorldPosition();
	glm::vec2 velocity{ 0, 0 };

	// Up - Down
	if (inputDir.y > 0 && m_pLevel->CanMoveInDirection(position, Level::Direction::Up) ||
		inputDir.y < 0 && m_pLevel->CanMoveInDirection(position, Level::Direction::Down))
	{
		velocity.y = inputDir.y * m_Speed;
	}

	// Left - Right
	if (inputDir.x < 0 && m_pLevel->CanMoveInDirection(position, Level::Direction::Left) ||
		inputDir.x > 0 && m_pLevel->CanMoveInDirection(position, Level::Direction::Right))
	{
		velocity.x = inputDir.x * m_Speed;
		GetGameObject()->GetTransform()->SetPosition(m_pLevel->SnapToPlatform(position));
	}

	const auto translation = velocity * Mage::Timer::GetInstance().GetDeltaTime();
	GetGameObject()->GetTransform()->Translate(translation);
}