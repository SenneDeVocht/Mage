#include "BurgerTime/BurgerTimePCH.h"
#include "PeterPepper.h"

#include "BurgerTime/PlayerAndEnemies/PlayerMovement.h"
#include "BurgerTime/Level.h"

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/AnimatedSpriteComponent.h"
#include "Mage/Components/BoxColliderComponent.h"
#include "Mage/Components/RigidBodyComponent.h"

PeterPepper::PeterPepper(Level* pLevel, const std::shared_ptr<Mage::SpriteAnimation>& pVictory, const std::shared_ptr<Mage::SpriteAnimation>& pDeath)
	: m_pLevel{ pLevel}
	, m_pVictory{ pVictory }
	, m_pDeath{ pDeath }
{}

void PeterPepper::Initialize()
{
	m_pMovement = GetGameObject()->GetComponentByType<PlayerMovement>();
	m_pRigidBody = GetGameObject()->GetComponentByType<Mage::RigidBodyComponent>();
	m_pAnimatedSprite = GetGameObject()->GetComponentByType<Mage::AnimatedSpriteComponent>();
}

void PeterPepper::Update()
{
	if (m_pLevel->IsCompleted())
	{
		m_pMovement->SetEnabled(false);
		m_pRigidBody->SetVelocity({ 0, 0 });

		if(m_pAnimatedSprite->GetAnimation() != m_pVictory.get())
			m_pAnimatedSprite->SetAnimation(m_pVictory);
	}
}

void PeterPepper::OnTriggerEnter(Mage::BoxColliderComponent* other)
{
	if (!m_IsDead && !m_pLevel->IsCompleted() && other->GetGameObject()->GetTag() == "Enemy")
	{
		m_IsDead = true;

		m_pMovement->SetEnabled(false);
		m_pRigidBody->SetVelocity({ 0, 0 });
		m_pAnimatedSprite->SetAnimation(m_pDeath);
	}
}
