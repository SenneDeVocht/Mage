#pragma once
#include "Component.h"

class Observer;
class Subject;

class PeterPepper final : public Component
{
public:
	explicit PeterPepper(int controllerIndex);
	~PeterPepper() override;

	PeterPepper(const PeterPepper& other) = delete;
	PeterPepper(PeterPepper&& other) = delete;
	PeterPepper& operator=(const PeterPepper& other) = delete;
	PeterPepper& operator=(PeterPepper&& other) = delete;

	void AddLivesObserver(Observer* observer);
	int GetLives();
	void Die();

	void AddPointsObserver(Observer* observer);
	int GetPoints();
	void GainPoints();

private:
	int m_Lives;
	std::unique_ptr<Subject> m_pLivesSubject;

	int m_Points;
	std::unique_ptr<Subject> m_pPointsSubject;
};

