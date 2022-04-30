#include "Mage/MagePCH.h"
#include "Mage/Components/BoxColliderComponent.h"

#pragma warning(push, 0)
#include <b2_fixture.h>
#pragma warning(pop)
#include "imgui.h"

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Scenegraph/Scene.h"
#include "Mage/Engine/PhysicsHandler.h"
#include "Mage/ImGui/ImGuiHelper.h"
#include "Mage/Components/Transform.h"
#include "Mage/Engine/Renderer.h"
#include "Mage/Engine/ServiceLocator.h"

Mage::BoxColliderComponent::BoxColliderComponent(const glm::vec2& size, const glm::vec2& offset, float angle, bool isTrigger)
	: m_Size(size)
	, m_Offset(offset)
    , m_Angle(angle)
	, m_IsTrigger(isTrigger)
{}

void Mage::BoxColliderComponent::Initialize()
{
	GetGameObject()->GetScene()->GetPhysicsHandler()->AddBoxCollider(this);
	m_PreviousSize = m_Size * GetGameObject()->GetTransform()->GetWorldScale();
	m_PreviousOffset = m_Offset;
	m_PreviousAngle = m_Angle;
}

void Mage::BoxColliderComponent::RenderGizmos() const
{
	const auto wPos = GetGameObject()->GetTransform()->GetWorldPosition() + m_Offset * GetGameObject()->GetTransform()->GetWorldScale();
	const auto wSize = GetGameObject()->GetTransform()->GetWorldScale() * m_Size;
	const auto wRot = GetGameObject()->GetTransform()->GetWorldRotation() + m_Angle;

	std::vector<glm::vec2> positions{
		{ -wSize.x / 2, -wSize.y / 2 },
		{ -wSize.x / 2, wSize.y / 2 },
		{ wSize.x / 2, wSize.y / 2 },
		{ wSize.x / 2, -wSize.y / 2 }
	};

	const float rotInRad = glm::radians(wRot);
	for (auto& pos : positions)
	{
		pos = glm::vec2 {
			pos.x * cos(rotInRad) - pos.y * sin(rotInRad),
			pos.x * sin(rotInRad) + pos.y * cos(rotInRad)
		};

		pos += wPos;
	}

	ServiceLocator::GetRenderer()->RenderPolygon(positions, glm::vec4(1.0f, 0.0f, 0.0f, 0.3f));
}

void Mage::BoxColliderComponent::DrawProperties()
{
	ImGuiHelper::Component("Box Collider Component", this, nullptr, [&]()
	{
		bool changed = false;

		ImGuiHelper::ItemLabel("Is Trigger", ImGuiHelper::ItemLabelAlignment::Left);
		changed |= ImGui::Checkbox("##IsTrigger", &m_IsTrigger);

		ImGuiHelper::ItemLabel("Size", ImGuiHelper::ItemLabelAlignment::Left);
		changed |= ImGui::DragFloat2("##Size", &m_Size.x, 0.1f);

		ImGuiHelper::ItemLabel("Offset", ImGuiHelper::ItemLabelAlignment::Left);
		changed |= ImGui::DragFloat2("##Offset", &m_Offset.x, 0.1f);

		ImGuiHelper::ItemLabel("Rotation", ImGuiHelper::ItemLabelAlignment::Left);
		changed |= ImGui::DragFloat("##Rotation", &m_Angle);

		if (changed)
			RecalculateShape();
	});
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

void Mage::BoxColliderComponent::RecalculateShape()
{
	if (m_Size == m_PreviousSize ||
		m_Offset == m_PreviousOffset ||
		m_Angle == m_PreviousAngle)
	{
		return;
	}

	const auto body = m_RunTimeFixture->GetBody();
	body->DestroyFixture(m_RunTimeFixture);

	GetGameObject()->GetScene()->GetPhysicsHandler()->AddBoxCollider(this);
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

