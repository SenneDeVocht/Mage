#pragma once

enum class Event
{
	PlayerDeath,
	EnemyDeath,
	BurgerDrop,
	LevelComplete
};

class Observer
{
public:
	virtual void OnNotify(Event event) = 0;
};

class Subject
{
public:
	void AddObserver(Observer* observer);
	void Notify(Event event);

private:
	std::vector<Observer*> m_Observers{};
};