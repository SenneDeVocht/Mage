#include "Mage/Components/Component.h"


namespace Mage
{
	class RigidBodyComponent;
	class AnimatedSpriteComponent;
	struct SpriteAnimation;
}

class EnemyManager;
class Level;
class PlayerMovement;

class PeterPepper final : public Mage::Component
{
public:
	PeterPepper(Level* pLevel, EnemyManager* pEnemyManager, const std::shared_ptr<Mage::SpriteAnimation>& pVictory, const std::shared_ptr<Mage::SpriteAnimation>& pDeath);

	void Initialize() override;
	void Update() override;
	void OnTriggerEnter(Mage::BoxColliderComponent* other) override;

	int GetPepperCount() const { return m_PepperCount; }

	int GetMaxLives() const { return m_MaxLives; }
	int GetLivesLeft() const { return m_LivesLeft; }

private:
	void SprayPepper();
	void StopSprayingPepper();

	void Die();
	void Reset();

	bool m_IsDead{ false };
	const float m_DeathDuration{ 3.0f };
	float m_DeathTimer{ 0.0f };

	Level* m_pLevel{};
	EnemyManager* m_pEnemyManager{};

	PlayerMovement* m_pMovement{};
	Mage::AnimatedSpriteComponent* m_pAnimatedSprite{};

	std::shared_ptr<Mage::SpriteAnimation> m_pVictory{};
	std::shared_ptr<Mage::SpriteAnimation> m_pDeath{};

	glm::vec2 m_StartPosition{};

	// Lives
	const int m_MaxLives{ 5 };
	int m_LivesLeft{ m_MaxLives };

	// PepperSpray
	int m_PepperCount{ 5 };
	bool m_SprayingPepper{ false };

	Mage::GameObject* m_pPepperSpray{};
	const float m_PepperSprayDuration{ 0.4f };
	float m_PepperSprayTimer{ 0 };

	std::shared_ptr<Mage::SpriteAnimation> m_pPepperSprayRightAnimation{};
	std::shared_ptr<Mage::SpriteAnimation> m_pPepperSprayLeftAnimation{};
	std::shared_ptr<Mage::SpriteAnimation> m_pPepperSprayUpAnimation{};
	std::shared_ptr<Mage::SpriteAnimation> m_pPepperSprayDownAnimation{};
};
