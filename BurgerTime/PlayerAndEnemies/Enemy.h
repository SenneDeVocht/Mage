#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
	class RigidBodyComponent;
	class AnimatedSpriteComponent;
	struct SpriteAnimation;
}

class Level;
class BurgerIngredient;

class Enemy final : public Mage::Component
{
public:
	Enemy(Component* movement, const std::shared_ptr<Mage::SpriteAnimation>& pDeath);

	void Initialize() override;
	void Update() override;
	void OnTriggerEnter(Mage::BoxColliderComponent* other) override;
	void OnTriggerExit(Mage::BoxColliderComponent* other) override;

private:
	void Die();

	Component* m_pMovement{};
	Mage::RigidBodyComponent* m_pRigidBody{};
	Mage::AnimatedSpriteComponent* m_pAnimatedSprite{};
	
	std::shared_ptr<Mage::SpriteAnimation> m_pDeath{};

	BurgerIngredient* m_pLastTouchedIngredient{};
	bool m_TouchingIngredient{ false };
	bool m_IsFalling{ false };

	float m_DeadTimer = 0.0f;
	bool m_IsDead{ false };
};