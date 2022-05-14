#include "Mage/Components/Component.h"

namespace Mage
{
	class RigidBodyComponent;
	class AnimatedSpriteComponent;
	struct SpriteAnimation;
}

class Level;
class PlayerMovement;

class PeterPepper final : public Mage::Component
{
public:
	PeterPepper(Level* pLevel, const std::shared_ptr<Mage::SpriteAnimation>& pVictory, const std::shared_ptr<Mage::SpriteAnimation>& pDeath);

	void Initialize() override;
	void Update() override;
	void OnTriggerEnter(Mage::BoxColliderComponent* other) override;

private:
	bool m_IsDead{ false };

	Level* m_pLevel{};

	PlayerMovement* m_pMovement{};
	Mage::RigidBodyComponent* m_pRigidBody{};
	Mage::AnimatedSpriteComponent* m_pAnimatedSprite{};

	std::shared_ptr<Mage::SpriteAnimation> m_pVictory{};
	std::shared_ptr<Mage::SpriteAnimation> m_pDeath{};
};
