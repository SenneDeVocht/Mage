#pragma once
#include "Mage/Components/Component.h"

class Level;

namespace Mage
{
	class RigidBodyComponent;
	class AnimatedSpriteComponent;
	struct SpriteAnimation;
}

class PlayerMovement final : public Mage::Component
{
public:
	PlayerMovement(Level* level, const std::shared_ptr<Mage::SpriteAnimation>& pIdle, const std::shared_ptr<Mage::SpriteAnimation>& pWalkfront,
		const std::shared_ptr<Mage::SpriteAnimation>& pWalkBack, const std::shared_ptr<Mage::SpriteAnimation>& pWalkLeft, const std::shared_ptr<Mage::SpriteAnimation>& pWalkRight);

	void Initialize() override;
	void Update() override;
	void FixedUpdate() override;
	void OnDisable() override;

private:
	Level* m_pLevel{};

	Mage::RigidBodyComponent* m_pRigidBody{};
	Mage::AnimatedSpriteComponent* m_pAnimatedSprite{};
	
	std::shared_ptr<Mage::SpriteAnimation> m_pIdle{};
	std::shared_ptr<Mage::SpriteAnimation> m_pWalkFront{};
	std::shared_ptr<Mage::SpriteAnimation> m_pWalkBack{};
	std::shared_ptr<Mage::SpriteAnimation> m_pWalkLeft{};
	std::shared_ptr<Mage::SpriteAnimation> m_pWalkRight{};

	glm::vec2 m_Input{ 0, 0 };
	float m_Speed{ 2 };
};

