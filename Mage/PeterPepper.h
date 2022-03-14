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

private:
	explicit PeterPepper();

	int m_Lives;
	std::unique_ptr<Subject> m_pLivesSubject;
};

