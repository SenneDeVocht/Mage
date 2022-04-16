#include "Mage/MagePCH.h"
#include "Mage/Components/RigidBodyComponent.h"

#pragma warning(push, 0)
#include <b2_body.h>
#pragma warning(pop)

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/Transform.h"
#include "Mage/Scenegraph/Scene.h"
#include "Mage/Engine/PhysicsHandler.h"

Mage::RigidBodyComponent::RigidBodyComponent(BodyType type, bool fixedRotation, float gravityScale)
	: m_Type{ type }
	, m_FixedRotation{ fixedRotation }
	, m_GravityScale{ gravityScale }
{}

void Mage::RigidBodyComponent::Initialize()
{
	GetGameObject()->GetScene()->GetPhysicsHandler()->AddRigidBody(this);
}

void Mage::RigidBodyComponent::FixedUpdate()
{
	GetGameObject()->GetTransform()->SetWorldPosition({ m_RunTimeBody->GetPosition().x, m_RunTimeBody->GetPosition().y });
}

void Mage::RigidBodyComponent::SetPosition(const glm::vec2& position)
{
	m_RunTimeBody->SetTransform(b2Vec2(position.x, position.y), 0);
}

void Mage::RigidBodyComponent::SetVelocity(const glm::vec2& velocity)
{
	m_RunTimeBody->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
}

glm::vec2 Mage::RigidBodyComponent::GetVelocity() const
{
	return { m_RunTimeBody->GetLinearVelocity().x, m_RunTimeBody->GetLinearVelocity().y };
}

void Mage::RigidBodyComponent::ApplyForce(const glm::vec2& force, bool wake)
{
	m_RunTimeBody->ApplyForceToCenter(b2Vec2(force.x, force.y), wake);
}

void Mage::RigidBodyComponent::ApplyImpulse(const glm::vec2& impulse, bool wake)
{
	m_RunTimeBody->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), wake);
}
