#include "BurgerTime/BurgerTimePCH.h"
#include "PlayerMovement.h"

#include "BurgerTime/Level.h"

#include "Mage/Input/InputManager.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/Transform.h"
#include "Mage/Components/AnimatedSpriteComponent.h"
#include "Mage/Components/RigidBodyComponent.h"
#include "Mage/Engine/ServiceLocator.h"

PlayerMovement::PlayerMovement(Level* level, const std::shared_ptr<Mage::SpriteAnimation>& pIdle, const std::shared_ptr<Mage::SpriteAnimation>& pWalkfront,
	const std::shared_ptr<Mage::SpriteAnimation>& pWalkBack, const std::shared_ptr<Mage::SpriteAnimation>& pWalkLeft, const std::shared_ptr<Mage::SpriteAnimation>& pWalkRight)
    : m_pLevel{ level }
	, m_pIdle{ pIdle }
	, m_pWalkFront{ pWalkfront }
	, m_pWalkBack{ pWalkBack }
	, m_pWalkLeft{ pWalkLeft }
	, m_pWalkRight{ pWalkRight }
{
	std::cout << "[PeterPepper] Arrow keys to move" << std::endl;
}

void PlayerMovement::Initialize()
{
	m_pRigidBody = GetGameObject()->GetComponent<Mage::RigidBodyComponent>();
	m_pAnimatedSprite = GetGameObject()->GetComponent<Mage::AnimatedSpriteComponent>();
}

void PlayerMovement::Update()
{
	// INPUT
	//------
	m_Input = { 0, 0 };
	
	if (Mage::ServiceLocator::GetInputManager()->CheckKeyboardKey(0x26, Mage::InputState::Hold))
		++m_Input.y;
	if (Mage::ServiceLocator::GetInputManager()->CheckKeyboardKey(0x28, Mage::InputState::Hold))
		--m_Input.y;

	if (m_Input.y == 0)
	{
		if (Mage::ServiceLocator::GetInputManager()->CheckKeyboardKey(0x25, Mage::InputState::Hold))
			--m_Input.x;
		if (Mage::ServiceLocator::GetInputManager()->CheckKeyboardKey(0x27, Mage::InputState::Hold))
			++m_Input.x;
	}

	// ANIMATIONS
	//-----------
	if (m_Input.x == 0 && m_Input.y == 0 && m_pAnimatedSprite->GetAnimation() != m_pIdle.get())
		m_pAnimatedSprite->SetAnimation(m_pIdle);
    
	else if (m_Input.x > 0 && m_pAnimatedSprite->GetAnimation() != m_pWalkRight.get())
		m_pAnimatedSprite->SetAnimation(m_pWalkRight);
    
	else if (m_Input.x < 0 && m_pAnimatedSprite->GetAnimation() != m_pWalkLeft.get())
		m_pAnimatedSprite->SetAnimation(m_pWalkLeft);
    
	else if (m_Input.y > 0 && m_pAnimatedSprite->GetAnimation() != m_pWalkBack.get())
		m_pAnimatedSprite->SetAnimation(m_pWalkBack);
    
	else if (m_Input.y < 0 && m_pAnimatedSprite->GetAnimation() != m_pWalkFront.get())
		m_pAnimatedSprite->SetAnimation(m_pWalkFront);
}

void PlayerMovement::FixedUpdate()
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
		m_LastDirection = { 0, m_Input.y };
	}

	// Left - Right
	if (m_Input.x < 0 && m_pLevel->CanMoveInDirection(position, Level::Direction::Left) ||
		m_Input.x > 0 && m_pLevel->CanMoveInDirection(position, Level::Direction::Right))
	{
		velocity.x = m_Input.x * m_Speed;
		m_LastDirection = { m_Input.x, 0 };
		GetGameObject()->GetTransform()->SetLocalPosition(m_pLevel->SnapToPlatform(position));
	}

	m_pRigidBody->SetVelocity(velocity);
}

void PlayerMovement::OnDisable()
{
	m_pRigidBody->SetVelocity({ 0, 0 });
}
