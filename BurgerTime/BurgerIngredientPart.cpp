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
		if (other->GetGameObject()->GetTag() == "PeterPepper" ||
			other->GetGameObject()->GetTag() == "Ingredient" &&
			other->GetGameObject() != m_pIngredientItsAPartOf->GetGameObject() &&
			other->GetGameObject()->GetParent() != m_pIngredientItsAPartOf->GetGameObject())
		{
			m_IsSteppedOn = true;
		}
	}
}

void BurgerIngredientPart::Update()
{
	if (m_IsSteppedOn && !m_IsMoved && !m_IsFalling)
	{
		GetGameObject()->GetTransform()->Translate({ 0, -m_StepOnDistance });
		m_IsMoved = true;

		m_pIngredientItsAPartOf->PartSteppedOn();
	}
}

void BurgerIngredientPart::ReadyForFall()
{
	const auto transform = GetGameObject()->GetTransform();
	transform->SetPosition({ transform->GetLocalPosition().x, 0 });

	m_IsMoved = false;
	m_IsSteppedOn = false;
	m_IsFalling = true;
}

void BurgerIngredientPart::DoneFalling()
{
	m_IsFalling = false;
}
