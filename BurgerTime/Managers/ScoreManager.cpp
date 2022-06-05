#include "BurgerTime/BurgerTimePCH.h"
#include "ScoreManager.h"

#include "GameManager.h"
#include "BurgerTime/Level.h"
#include "BurgerTime/PlayerAndEnemies/EnemyManager.h"

ScoreManager::ScoreManager(GameManager* gameManager, Level* pLevel, EnemyManager* pEnemyManager)
{
	gameManager->RegisterScoreManager(this);
	pLevel->AddBurgerObserver(this);
	pEnemyManager->AddEnemyObserver(this);
}

void ScoreManager::OnNotify(Event event)
{
	if (m_ScoreEvents.find(event) != m_ScoreEvents.end())
		m_Score += m_ScoreEvents[event];
}
