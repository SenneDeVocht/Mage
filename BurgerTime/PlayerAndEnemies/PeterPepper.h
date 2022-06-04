#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
	class RigidBodyComponent;
	class AnimatedSpriteComponent;
	struct SpriteAnimation;
}

class GameManager;
class EnemyManager;
class Level;
class PlayerMovement;

class PeterPepper final : public Mage::Component
{
public:
	PeterPepper(GameManager* pGameManager, Level* pLevel, const std::shared_ptr<Mage::SpriteAnimation>& pVictory, const std::shared_ptr<Mage::SpriteAnimation>& pDeath);

	void Initialize() override;
	void Update() override;
	void OnTriggerEnter(Mage::BoxColliderComponent* other) override;

	int GetPepperCount() const { return m_PepperCount; }

	int GetMaxLives() const { return m_MaxLives; }
	int GetLivesLeft() const { return m_LivesLeft; }

	void StartVictory();
	void Reset();

private:
	enum class State
	{
		Alive,
		Dead,
		Victory
	};

	void SprayPepper();
	void StopSprayingPepper();

	void Die();

	GameManager* m_pGameManager{};
	Level* m_pLevel{};

	State m_State{ State::Alive };

	const float m_DeathDuration{ 3.0f };
	float m_DeathTimer{ 0.0f };

	PlayerMovement* m_pMovement{};
	Mage::AnimatedSpriteComponent* m_pAnimatedSprite{};

	std::shared_ptr<Mage::SpriteAnimation> m_pVictory{};
	std::shared_ptr<Mage::SpriteAnimation> m_pDeath{};

	// Lives
	const int m_MaxLives{ 3 };
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
