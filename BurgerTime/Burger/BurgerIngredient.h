#pragma once
#include "Mage\Components\Component.h"

namespace Mage
{
    class RigidBodyComponent;
}

class Subject;
class Observer;
class Level;
class BurgerIngredientPart;

class BurgerIngredient : public Mage::Component
{
public:
	enum class IngredientType
	{
		BunTop,
		BunBottom,
		Cheese,
		Patty,
		Tomato,
		Salad
	};

	BurgerIngredient(Level* level, IngredientType type);

	void Initialize() override;
	void Update() override;
	void OnTriggerEnter(Mage::BoxColliderComponent* other) override;

	void AddObserver(Observer* observer);

	void PartSteppedOn();
	bool IsFalling() const { return m_Falling; }
	bool IsCollected() const { return m_KeepFalling; }

private:
	void StartFalling();

	std::unique_ptr<Subject> m_pSubject;

	Level* m_pLevel;
	IngredientType m_Type;

	int m_PartsSteppedOn{ 0 };
	std::vector<BurgerIngredientPart*> m_Parts;

	Mage::RigidBodyComponent* m_pRigidBody{ nullptr };

	bool m_Falling{ false };
	bool m_KeepFalling{ false };
	float m_FallDestination{ 0 };
};

