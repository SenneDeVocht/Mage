#pragma once
#include "Component.h"

class Observer;
class Subject;

class PeterPepper final : public Component
{
	template<typename componentType, typename... argTypes>
	friend componentType* GameObject::CreateComponent(argTypes&&... args);

public:
	~PeterPepper() override;
	PeterPepper(const PeterPepper& other) = delete;
	PeterPepper(PeterPepper&& other) = delete;
	PeterPepper& operator=(const PeterPepper& other) = delete;
	PeterPepper& operator=(PeterPepper&& other) = delete;

	void Update() override {};
	void FixedUpdate() override {}
	void Render() const override {}

	void AddLivesObserver(Observer* observer);
	int GetLives();
	void Die();

	void AddPointsObserver(Observer* observer);
	int GetPoints();
	void GainPoints();

private:
	explicit PeterPepper(int controllerIndex);

	int m_Lives;
	std::unique_ptr<Subject> m_pLivesSubject;

	int m_Points;
	std::unique_ptr<Subject> m_pPointsSubject;
};

