#pragma once
#include <map>

#include "Mage/Singleton.h"

class ScoreManager : public Singleton<ScoreManager>
{
public:
	enum class ScoreEvent
	{
		EnemyKilled,
		BurgerDropped
	};

	int GetScore() const { return m_Score; }
	void TriggerScoreEvent(ScoreEvent eventType);

private:
	friend class Singleton<ScoreManager>;
	ScoreManager() = default;

	int m_Score;

	std::map<ScoreEvent, int> m_ScoreEvents{
		std::make_pair(ScoreEvent::EnemyKilled,   200),
		std::make_pair(ScoreEvent::BurgerDropped, 50)
	};
};

