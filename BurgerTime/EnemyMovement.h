#pragma once
#include "Mage/Components/Component.h"

class Level;

namespace Mage
{
	class Transform;
    class RigidBodyComponent;
    class AnimatedSpriteComponent;
	struct SpriteAnimation;
}

class EnemyMovement final : public Mage::Component
{
public:
	EnemyMovement(Level* level, Mage::Transform* target, const std::shared_ptr<Mage::SpriteAnimation>& pWalkfront, const std::shared_ptr<Mage::SpriteAnimation>& pWalkBack,
		const std::shared_ptr<Mage::SpriteAnimation>& pWalkLeft, const std::shared_ptr<Mage::SpriteAnimation>& pWalkRight);

	void Initialize() override;
	void FixedUpdate() override;

private:
	Mage::RigidBodyComponent* m_pRigidBody{};
	Mage::AnimatedSpriteComponent* m_pAnimatedSprite{};

	Level* m_pLevel{};
    Mage::Transform* m_pTarget{};
	
	std::shared_ptr<Mage::SpriteAnimation> m_pWalkFront{};
	std::shared_ptr<Mage::SpriteAnimation> m_pWalkBack{};
	std::shared_ptr<Mage::SpriteAnimation> m_pWalkLeft{};
	std::shared_ptr<Mage::SpriteAnimation> m_pWalkRight{};

	const float m_LadderThreshold{ 0.1f };

	int m_LastIntersection{ -1 };
	glm::ivec2 m_CurrentDirection{ 0, 0 };
	float m_Speed{ 2 };
};
