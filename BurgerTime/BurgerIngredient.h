#pragma once
#include "Mage\Components\Component.h"

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

	void PartSteppedOn();

private:
	Level* m_pLevel;
	IngredientType m_Type;

	int m_PartsSteppedOn{ 0 };
	std::vector<BurgerIngredientPart*> m_Parts;

	bool m_Falling;
	float m_FallDestination;
	glm::vec2 m_FallVelocity{ 0, 0 };
	const glm::vec2 m_FallAcceleration{ 0, -9.81f };
};

