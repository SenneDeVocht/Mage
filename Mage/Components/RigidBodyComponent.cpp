#include "Mage/MagePCH.h"
#include "Mage/Components/RigidBodyComponent.h"

#include <b2_body.h>

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/Transform.h"
#include "Mage/Scenegraph/Scene.h"

Mage::RigidBodyComponent::RigidBodyComponent(BodyType type, bool fixedRotation)
	: m_Type{ type }
	, m_FixedRotation{ fixedRotation }
{}

void Mage::RigidBodyComponent::Initialize()
{
	GetGameObject()->GetScene()->AddRigidBody(this);
}

void Mage::RigidBodyComponent::FixedUpdate()
{
	const b2Body* body = static_cast<b2Body*>(m_RunTimeBody);
	GetGameObject()->GetTransform()->SetPosition({ body->GetPosition().x, body->GetPosition().y });
}
