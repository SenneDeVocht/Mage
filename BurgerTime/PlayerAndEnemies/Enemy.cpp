#include "BurgerTime/BurgerTimePCH.h"
#include "Enemy.h"

#include "BurgerTime/Burger/BurgerIngredient.h"
#include "Mage/Components/AnimatedSpriteComponent.h"
#include "Mage/Components/BoxColliderComponent.h"
#include "Mage/Components/RigidBodyComponent.h"
#include "Mage/Components/Transform.h"
#include "Mage/Engine/Timer.h"
#include "Mage/Scenegraph/GameObject.h"

Enemy::Enemy(Component* movement, const std::shared_ptr<Mage::SpriteAnimation>& pDeath)
	: m_pMovement{ movement }
	, m_pDeath{ pDeath }
{}

void Enemy::Initialize()
{
	m_pRigidBody = GetGameObject()->GetComponentByType<Mage::RigidBodyComponent>();
	m_pAnimatedSprite = GetGameObject()->GetComponentByType<Mage::AnimatedSpriteComponent>();
}

void Enemy::Update()
{
	if (m_IsDead)
	{
		m_DeadTimer += Mage::Timer::GetInstance().GetDeltaTime();

		if (m_DeadTimer >= m_pDeath->NumFrames * m_pDeath->SecondsPerFrame)
			GetGameObject()->Destroy();

		return;
	}
	
	if (m_pLastTouchedIngredient != nullptr && m_TouchingIngredient && m_pLastTouchedIngredient->IsFalling() && !m_IsFalling && !m_IsDead &&
		m_pLastTouchedIngredient->GetGameObject()->GetTransform()->GetWorldPosition().y < GetGameObject()->GetTransform()->GetWorldPosition().y)
	{
		m_IsFalling = true;
		m_pMovement->SetEnabled(false);
		m_pRigidBody->SetGravityScale(1);
	}

	if (m_IsFalling &&
		m_pLastTouchedIngredient != nullptr &&
		!m_pLastTouchedIngredient->IsFalling())
	{
		Die();
	}
}


void Enemy::OnTriggerEnter(Mage::BoxColliderComponent* other)
{
	const auto otherGameObject = other->GetGameObject();

	if (!m_IsDead && otherGameObject->GetTag() == "Ingredient")
	{
		m_pLastTouchedIngredient = otherGameObject->GetComponentByType<BurgerIngredient>();
		m_TouchingIngredient = true;
		std::cout << "Enemy touched ingredient" << std::endl;

		const bool isHigher = otherGameObject->GetTransform()->GetWorldPosition().y > GetGameObject()->GetTransform()->GetWorldPosition().y;
		if (m_pLastTouchedIngredient->IsFalling() && isHigher)
		{
			Die();
		}
	}
}

void Enemy::OnTriggerExit(Mage::BoxColliderComponent* other)
{
	if (other->GetGameObject()->GetTag() == "Ingredient")
	{
		m_TouchingIngredient = false;
		std::cout << "Enemy left ingredient" << std::endl;
	}
}

void Enemy::Die()
{
	m_IsDead = true;

	m_pMovement->SetEnabled(false);
	m_pRigidBody->SetGravityScale(0);
	m_pRigidBody->SetVelocity({ 0, 0 });
	m_pAnimatedSprite->SetAnimation(m_pDeath);
}