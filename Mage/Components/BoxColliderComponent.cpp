#include "Mage/MagePCH.h"
#include "Mage/Components/BoxColliderComponent.h"

#include <b2_fixture.h>

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Scenegraph/Scene.h"
#include "Mage/Engine/PhysicsHandler.h"

Mage::BoxColliderComponent::BoxColliderComponent(const glm::vec2& size, const glm::vec2& offset, bool isTrigger)
	: m_Size(size)
	, m_Offset(offset)
	, m_IsTrigger(isTrigger)
{}

void Mage::BoxColliderComponent::Initialize()
{
	GetGameObject()->GetScene()->GetPhysicsHandler()->AddBoxCollider(this);
}

void Mage::BoxColliderComponent::SetEnabled(bool enabled)
{
	Component::SetEnabled(enabled);

	if (!enabled)
	{
		b2Filter filter;
		filter.maskBits = 0;
		m_RunTimeFixture->SetFilterData(filter);
	}
	else
	{
		b2Filter filter;
		filter.maskBits = 1;
		m_RunTimeFixture->SetFilterData(filter);
	}
}


void Mage::BoxColliderComponent::NotifyGameObjectOnTriggerEnter(BoxColliderComponent* other) const
{
	GetGameObject()->OnTriggerEnter(other);
}

void Mage::BoxColliderComponent::NotifyGameObjectOnTriggerExit(BoxColliderComponent* other) const
{
	GetGameObject()->OnTriggerExit(other);
}

void Mage::BoxColliderComponent::NotifyGameObjectOnCollisionEnter(BoxColliderComponent* other) const
{
	GetGameObject()->OnCollisionEnter(other);
}

void Mage::BoxColliderComponent::NotifyGameObjectOnCollisionExit(BoxColliderComponent* other) const
{
	GetGameObject()->OnCollisionExit(other);
}

