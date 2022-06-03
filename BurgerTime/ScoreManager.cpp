#include "BurgerTime/BurgerTimePCH.h"
#include "ScoreManager.h"

void ScoreManager::TriggerScoreEvent(ScoreEvent eventType)
{
	m_Score += m_ScoreEvents[eventType];
}
