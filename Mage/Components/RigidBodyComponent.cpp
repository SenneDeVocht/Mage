#include "Mage/MagePCH.h"
#include "Mage/Components/RigidBodyComponent.h"

#pragma warning(push, 0)
#include <b2_body.h>
#include <b2_polygon_shape.h>
#include <b2_fixture.h>
#pragma warning(pop)

#include "imgui.h"

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/Transform.h"
#include "Mage/Scenegraph/Scene.h"
#include "Mage/Engine/PhysicsHandler.h"
#include "Mage/ImGui/ImGuiHelper.h"
#include "Mage/Components/BoxColliderComponent.h"

Mage::RigidBodyComponent::RigidBodyComponent(BodyType type, bool fixedRotation, float gravityScale)
	: m_InitialType{ type }
    , m_InitialFixedRotation{ fixedRotation }
    , m_InitialGravityScale{ gravityScale }
{}

void Mage::RigidBodyComponent::Initialize()
{
	GetGameObject()->GetScene()->GetPhysicsHandler()->AddRigidBody(this, static_cast<int>(m_InitialType), m_InitialFixedRotation, m_InitialGravityScale);
	TransformChanged();
	NotifyBoxCollidersOfChange(GetGameObject());
}

void Mage::RigidBodyComponent::DrawProperties()
{
	ImGuiHelper::Component("Rigidbody Component", this, &m_ShouldBeEnabled, [&]()
	{  
	    int type = static_cast<int>(GetType());
		ImGuiHelper::ItemLabel("Type", ImGuiHelper::ItemLabelAlignment::Left);
		if(ImGui::Combo("##Type", &type, "Static\0Kinematic\0Dynamic\0"))
			SetType(static_cast<BodyType>(type));

		bool fixedRotation = GetFixedRotation();
		ImGuiHelper::ItemLabel("Fix Rotation", ImGuiHelper::ItemLabelAlignment::Left);
		if(ImGui::Checkbox("##FixRotation", &fixedRotation))
			SetFixedRotation(fixedRotation);

		float gravityScale = GetGravityScale();
		ImGuiHelper::ItemLabel("Gravity Scale", ImGuiHelper::ItemLabelAlignment::Left);
		if(ImGui::DragFloat("##GravityScale", &gravityScale, 0.1f))
			SetGravityScale(gravityScale);

		ImGui::BeginDisabled();

		glm::vec2 velocity = GetVelocity();
		ImGuiHelper::ItemLabel("Velocity", ImGuiHelper::ItemLabelAlignment::Left);
		ImGui::DragFloat2("##Velocity", &velocity.x);

		ImGui::EndDisabled();
	});
}

void Mage::RigidBodyComponent::OnDestroy()
{
	GetGameObject()->GetScene()->GetPhysicsHandler()->RemoveRigidBody(this);
	NotifyBoxCollidersOfChange(GetGameObject());
}

void Mage::RigidBodyComponent::NotifyBoxCollidersOfChange(const GameObject* gameObject) const
{
	for (const auto& col : gameObject->GetComponents<BoxColliderComponent>())
	{
		col->RigidBodyChanged();
	}

	for (const auto& child : gameObject->GetChildren())
    {
		if (child->GetComponent<RigidBodyComponent>() != nullptr)
           NotifyBoxCollidersOfChange(child);
    }
}

void Mage::RigidBodyComponent::AddBoxCollider(BoxColliderComponent* boxCollider) const
{
	const auto objectScale = boxCollider->GetGameObject()->GetTransform()->GetWorldScale();
	const auto objectOffset = boxCollider->GetGameObject()->GetTransform()->GetWorldPosition() - GetGameObject()->GetTransform()->GetWorldPosition();
	const auto objectRotationOffset = boxCollider->GetGameObject()->GetTransform()->GetWorldRotation() - GetGameObject()->GetTransform()->GetWorldRotation();

	b2PolygonShape boxShape;
	boxShape.SetAsBox(
		boxCollider->GetSize().x / 2.f * objectScale.x, boxCollider->GetSize().y / 2.f * objectScale.y,
		{ objectOffset.x + boxCollider->GetOffset().x * objectScale.x, objectOffset.y + boxCollider->GetOffset().y * objectScale.y },
		boxCollider->GetRotation() + objectRotationOffset);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.isSensor = boxCollider->IsTrigger();
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(boxCollider);

	// TODO: Physics Material
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.5f;
	fixtureDef.restitution = 0.5f;
	fixtureDef.restitutionThreshold = 0.5f;

	const auto fixture = m_RunTimeBody->CreateFixture(&fixtureDef);

	boxCollider->SetRunTimeFixture(fixture);
}

void Mage::RigidBodyComponent::RemoveBoxCollider(BoxColliderComponent* boxCollider) const
{
    m_RunTimeBody->DestroyFixture(boxCollider->GetRunTimeFixture());
}

void Mage::RigidBodyComponent::TransformChanged() const
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

Mage::RigidBodyComponent::BodyType Mage::RigidBodyComponent::GetType() const
{
    return static_cast<BodyType>(m_RunTimeBody->GetType());
}

void Mage::RigidBodyComponent::SetType(BodyType type) const
{
    m_RunTimeBody->SetType(static_cast<b2BodyType>(type));
	m_RunTimeBody->SetAwake(true);
}

bool Mage::RigidBodyComponent::GetFixedRotation() const
{
    return m_RunTimeBody->IsFixedRotation();
}

void Mage::RigidBodyComponent::SetFixedRotation(bool fixedRotation) const
{
    m_RunTimeBody->SetFixedRotation(fixedRotation);
	m_RunTimeBody->SetAwake(true);
}

float Mage::RigidBodyComponent::GetGravityScale() const
{
	return m_RunTimeBody->GetGravityScale();
}

void Mage::RigidBodyComponent::SetGravityScale(float gravityScale) const
{
    m_RunTimeBody->SetGravityScale(gravityScale);
	m_RunTimeBody->SetAwake(true);
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
