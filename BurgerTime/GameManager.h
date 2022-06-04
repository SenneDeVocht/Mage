#pragma once
#include "Mage/Components/Component.h"

class Level;
class EnemyManager;
class PeterPepper;

class GameManager : public Mage::Component
{
public:
	void Update() override;

	void RegisterLevel(Level* pLevel){ m_pLevel = pLevel; }
	void RegisterPeterPepper(PeterPepper* pPeterPepper) { m_pPeterPepper = pPeterPepper; }
	void RegisterEnemyManager(EnemyManager* pEnemyManager) { m_pEnemyManager = pEnemyManager; }

	void OnPlayerDied(bool wasLastLife);
	void OnLevelCompleted();

private:
	void GoToNextLevel();

	Level* m_pLevel{};
	PeterPepper* m_pPeterPepper{};
	EnemyManager* m_pEnemyManager{};

	int m_CurrentLevel{ 1 };

	bool m_LevelCompleted{ false };
	const float m_VictoryLength{ 5.0f };
	float m_VictoryTimer{ 0.0f };
};

