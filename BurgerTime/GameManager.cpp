#include "BurgerTime/BurgerTimePCH.h"
#include "GameManager.h"

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
			GoToNextLevel();
		}
	}	
}

void GameManager::OnPlayerDied(bool wasLastLife)
{
	if (wasLastLife)
	{
        Mage::SceneManager::GetInstance().LoadScene("GameOver");
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

	m_pLevel->LoadLevel(m_CurrentLevel);

	m_pPeterPepper->Reset();
	m_pEnemyManager->Reset();
}

