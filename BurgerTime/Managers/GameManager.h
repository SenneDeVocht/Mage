#pragma once
#include "Mage/Components/Component.h"

class ScoreManager;
class Level;
class EnemyManager;
class PeterPepper;

class GameManager : public Mage::Component
{
public:
	void Initialize() override;
	void Update() override;

	void RegisterLevel(Level* pLevel){ m_pLevel = pLevel; }
	void RegisterEnemyManager(EnemyManager* pEnemyManager) { m_pEnemyManager = pEnemyManager; }
	void RegisterScoreManager(ScoreManager* pScoreManager) { m_pScoreManager = pScoreManager; }
	
	void OnLevelCompleted();

	int GetMaxLives() const { return m_MaxLives; }
	int GetLivesLeft() const { return m_LivesLeft; }

private:
	void SpawnPlayer(int playerIndex);

	void GoToNextLevel();
	void GameOver();

	Level* m_pLevel{};
	std::vector<PeterPepper*> m_pPeterPeppers{};
	EnemyManager* m_pEnemyManager{};
	ScoreManager* m_pScoreManager{};

	int m_CurrentLevel{ 1 };

	bool m_LevelCompleted{ false };
	const float m_VictoryLength{ 5.0f };
	float m_VictoryTimer{ 0.0f };

	// Lives
	const int m_MaxLives{ 3 };
	int m_LivesLeft{ m_MaxLives };
};

