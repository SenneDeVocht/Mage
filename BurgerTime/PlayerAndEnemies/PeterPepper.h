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
	PeterPepper(int playerIndex, Level* pLevel, const std::shared_ptr<Mage::SpriteAnimation>& pVictory, const std::shared_ptr<Mage::SpriteAnimation>& pDeath);

	void Initialize() override;
	void Update() override;
	void OnTriggerEnter(Mage::BoxColliderComponent* other) override;

	int GetPepperCount() const { return m_PepperCount; }

	void StartVictory();
	void Reset();

	bool IsDeadAndReady() const { return m_DeadAndReady; }

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

	int m_PlayerIndex;
	
	Level* m_pLevel{};

	State m_State{ State::Alive };

	const float m_DeathDuration{ 3.0f };
	float m_DeathTimer{ 0.0f };
	bool m_DeadAndReady{ false };

	PlayerMovement* m_pMovement{};
	Mage::AnimatedSpriteComponent* m_pAnimatedSprite{};

	std::shared_ptr<Mage::SpriteAnimation> m_pVictory{};
	std::shared_ptr<Mage::SpriteAnimation> m_pDeath{};

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
