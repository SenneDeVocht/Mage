#include "Mage/MagePCH.h"
#include "Mage/Components/BoxColliderComponent.h"

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Scenegraph/Scene.h"

Mage::BoxColliderComponent::BoxColliderComponent()
{
}

void Mage::BoxColliderComponent::Initialize()
{
	GetGameObject()->GetScene()->AddBoxCollider(this);	
}
