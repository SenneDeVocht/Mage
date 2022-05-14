#include "BurgerTime/BurgerTimePCH.h"
#include "BurgerIngredientPart.h"

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/BoxColliderComponent.h"
#include "Mage/Components/Transform.h"

#include "BurgerIngredient.h"

BurgerIngredientPart::BurgerIngredientPart(BurgerIngredient* ingredientItsAPartOf)
	: m_pIngredientItsAPartOf{ingredientItsAPartOf}
{}

void BurgerIngredientPart::OnTriggerEnter(Mage::BoxColliderComponent* other)
{
	if (!m_IsSteppedOn && !m_IsFalling)
	{
		if (other->GetGameObject()->GetTag() == "PeterPepper")
		{
			m_IsSteppedOn = true;
			GetGameObject()->GetTransform()->Translate({ 0, -m_StepOnDistance });

			m_pIngredientItsAPartOf->PartSteppedOn();
		}
	}
}
void BurgerIngredientPart::ReadyForFall()
{
	const auto transform = GetGameObject()->GetTransform();
	transform->SetLocalPosition({ transform->GetLocalPosition().x, 0 });
	
	m_IsSteppedOn = false;
	m_IsFalling = true;
}

void BurgerIngredientPart::DoneFalling()
{
	m_IsFalling = false;
}
