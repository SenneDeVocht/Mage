#pragma once
#include "Mage\Components\Component.h"

class BurgerIngredient;

class BurgerIngredientPart : public Mage::Component
{
public:
	BurgerIngredientPart(BurgerIngredient* ingredientItsAPartOf);

	void OnTriggerEnter(Mage::BoxColliderComponent* other) override;
	void Update() override;

	void ReadyForFall();
	void DoneFalling();

private:
	BurgerIngredient* m_pIngredientItsAPartOf{};
	bool m_IsSteppedOn{ false };
	bool m_IsMoved{ false };
	bool m_IsFalling{ false };

	// Drops 2 pixels when stepped on
	const float m_StepOnDistance{ 2.f / 16.f };
};
