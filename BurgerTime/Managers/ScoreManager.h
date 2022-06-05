#pragma once
#include <map>

#include "BurgerTime/EventSystem.h"
#include "Mage/Components/Component.h"

class GameManager;
class Level;
class EnemyManager;

class ScoreManager : public Mage::Component, public Observer
{
public:
	ScoreManager(GameManager* gameManager, Level* pLevel, EnemyManager* pEnemyManager);

	void OnNotify(Event event) override;

	int GetScore() const { return m_Score; }

private:
	int m_Score{ 0 };

	std::map<Event, int> m_ScoreEvents{
		std::make_pair(Event::EnemyDeath, 200),
		std::make_pair(Event::BurgerDrop, 50)
	};
};

