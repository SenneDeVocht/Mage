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
#include "Mage/ImGui/ImGuiHelper.h"

Mage::RigidBodyComponent::RigidBodyComponent(BodyType type, bool fixedRotation, float gravityScale)
	: m_Type{ type }
	, m_FixedRotation{ fixedRotation }
	, m_GravityScale{ gravityScale }
{}

Mage::RigidBodyComponent::~RigidBodyComponent()
{
	GetGameObject()->GetScene()->GetPhysicsHandler()->RemoveRigidBody(this);
}

void Mage::RigidBodyComponent::Awake()
{
	GetGameObject()->GetScene()->GetPhysicsHandler()->AddRigidBody(this);
	TransformChanged();
}

void Mage::RigidBodyComponent::DrawProperties()
{
	Mage::ImGuiHelper::Component("Rigidbody Component", this, &m_ShouldBeEnabled, [&]()
	{
		// TODO: add type and gravityscale

		ImGui::BeginDisabled();

		glm::vec2 velocity = GetVelocity();
		ImGuiHelper::ItemLabel("Velocity", ImGuiHelper::ItemLabelAlignment::Left);
		ImGui::DragFloat2("##Velocity", &velocity.x);

		ImGui::EndDisabled();
	});
}

void Mage::RigidBodyComponent::TransformChanged()
{
	const auto transform = GetGameObject()->GetTransform();
	m_RunTimeBody->SetTransform(
		b2Vec2(transform->GetWorldPosition().x, transform->GetWorldPosition().y), 
		glm::radians(transform->GetWorldRotation())
	);
	m_RunTimeBody->SetAwake(true);
}

void Mage::RigidBodyComponent::UpdateTransform() const
{
	GetGameObject()->GetTransform()->SetWorldPositionWithoutUpdatingRigidBody({ m_RunTimeBody->GetPosition().x, m_RunTimeBody->GetPosition().y });
	GetGameObject()->GetTransform()->SetWorldRotationWithoutUpdatingRigidBody(glm::degrees(m_RunTimeBody->GetAngle()));
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
