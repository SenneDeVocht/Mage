#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
	class RigidBodyComponent;
	class AnimatedSpriteComponent;
	struct SpriteAnimation;
}

class Subject;
class Observer;
class Level;
class BurgerIngredient;

class Enemy final : public Mage::Component
{
public:
	Enemy(Component* movement, const std::shared_ptr<Mage::SpriteAnimation>& pStunned, const std::shared_ptr<Mage::SpriteAnimation>& pDeath);
	~Enemy();

	void Initialize() override;
	void Update() override;
	void OnTriggerEnter(Mage::BoxColliderComponent* other) override;
	void OnTriggerExit(Mage::BoxColliderComponent* other) override;

	bool IsDead() const { return m_IsDead; }

	void AddObserver(Observer* observer);

private:
	void Die();

	std::unique_ptr<Subject> m_pSubject;

	Component* m_pMovement{};
	Mage::RigidBodyComponent* m_pRigidBody{};
	Mage::AnimatedSpriteComponent* m_pAnimatedSprite{};
	Mage::BoxColliderComponent* m_pCollider{};
	
	std::shared_ptr<Mage::SpriteAnimation> m_pStunned{};
	std::shared_ptr<Mage::SpriteAnimation> m_pDeath{};

	bool m_IsStunned{ false };
	const float m_StunnedDuration{ 2.0f };
	float m_StunnedTimer{ 0.0f };

	BurgerIngredient* m_pLastTouchedIngredient{};
	bool m_TouchingIngredient{ false };
	bool m_IsFalling{ false };

	float m_DeadTimer = 0.0f;
	bool m_IsDead{ false };
};
