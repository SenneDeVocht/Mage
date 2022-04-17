#include "Mage/MagePCH.h"
#include "Mage/Components/RigidBodyComponent.h"

#pragma warning(push, 0)
#include <b2_body.h>
#pragma warning(pop)

#include "imgui.h"

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/Transform.h"
#include "Mage/Scenegraph/Scene.h"
#include "Mage/Engine/PhysicsHandler.h"

Mage::RigidBodyComponent::RigidBodyComponent(BodyType type, bool fixedRotation, float gravityScale)
	: m_Type{ type }
	, m_FixedRotation{ fixedRotation }
	, m_GravityScale{ gravityScale }
{}

Mage::RigidBodyComponent::~RigidBodyComponent()
{
	GetGameObject()->GetScene()->GetPhysicsHandler()->RemoveRigidBody(this);
}

void Mage::RigidBodyComponent::Initialize()
{
	GetGameObject()->GetScene()->GetPhysicsHandler()->AddRigidBody(this);
}

void Mage::RigidBodyComponent::DrawProperties()
{
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader("Rigidbody Component"))
	{
		ImGui::Checkbox("Enabled", &m_ShouldBeEnabled);

		// TODO: add type and gravityscale

		ImGui::BeginDisabled();
		glm::vec2 velocity = GetVelocity();
		ImGui::DragFloat2("Velocity", &velocity.x);
		ImGui::EndDisabled();
	}

	ImGui::PopID();
}

void Mage::RigidBodyComponent::UpdateTransform() const
{
	GetGameObject()->GetTransform()->SetWorldPosition({ m_RunTimeBody->GetPosition().x, m_RunTimeBody->GetPosition().y });
}

void Mage::RigidBodyComponent::SetPosition(const glm::vec2& position) const
{
	m_RunTimeBody->SetTransform(b2Vec2(position.x, position.y), 0);
}

void Mage::RigidBodyComponent::SetVelocity(const glm::vec2& velocity) const
{
	m_RunTimeBody->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
}

glm::vec2 Mage::RigidBodyComponent::GetVelocity() const
{
	return { m_RunTimeBody->GetLinearVelocity().x, m_RunTimeBody->GetLinearVelocity().y };
}

void Mage::RigidBodyComponent::ApplyForce(const glm::vec2& force, bool wake) const
{
	m_RunTimeBody->ApplyForceToCenter(b2Vec2(force.x, force.y), wake);
}

void Mage::RigidBodyComponent::ApplyImpulse(const glm::vec2& impulse, bool wake) const
{
	m_RunTimeBody->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), wake);
}
