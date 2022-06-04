#include "BurgerTime/BurgerTimePCH.h"
#include "GameManager.h"

#include "SavedScoreManager.h"
#include "ScoreManager.h"
#include "BurgerTime/Level.h"
#include "BurgerTime/PlayerAndEnemies/PeterPepper.h"
#include "BurgerTime/PlayerAndEnemies/EnemyManager.h"

#include "Mage/Engine/Timer.h"
#include "Mage/Scenegraph/SceneManager.h"

void GameManager::Update()
{
	if (m_LevelCompleted)
	{
		m_VictoryTimer += Mage::Timer::GetInstance().GetDeltaTime();

		if (m_VictoryTimer >= m_VictoryLength)
		{
			m_LevelCompleted = false;
			m_VictoryTimer = 0.0f;
			GoToNextLevel();
		}
	}	
}

void GameManager::OnPlayerDied(bool wasLastLife)
{
	if (wasLastLife)
	{
		GameOver();
        return;
	}

	m_pPeterPepper->Reset();
	m_pEnemyManager->Reset();
}

void GameManager::OnLevelCompleted()
{
	m_LevelCompleted = true;
	m_pPeterPepper->StartVictory();
}

void GameManager::GoToNextLevel()
{
	++m_CurrentLevel;

	if (m_CurrentLevel > 3)
	{
		GameOver();
	}
	else
	{
		m_pLevel->LoadLevel(m_CurrentLevel);

		m_pPeterPepper->Reset();
		m_pEnemyManager->Reset();
	}
}

void GameManager::GameOver()
{
	const int score = m_pScoreManager->GetScore();
	if (score > SavedScoreManager::GetSavedScore())
	{
		SavedScoreManager::SetSavedScore(score);
	}

	Mage::SceneManager::GetInstance().LoadScene("GameOver");
}
