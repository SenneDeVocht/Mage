#include "Mage/MagePCH.h"
#include "Mage/Components/BoxColliderComponent.h"

#pragma warning(push, 0)
#include <b2_fixture.h>
#pragma warning(pop)
#include "imgui.h"
#include "RigidBodyComponent.h"

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/ImGui/ImGuiHelper.h"
#include "Mage/Components/Transform.h"
#include "Mage/Engine/Renderer.h"
#include "Mage/Engine/ServiceLocator.h"

Mage::BoxColliderComponent::BoxColliderComponent(const glm::vec2& size, const glm::vec2& offset, float angle, bool isTrigger)
	: m_Size(size)
	, m_Offset(offset)
    , m_Rotation(angle)
	, m_InitialIsTrigger{ isTrigger }
{}

void Mage::BoxColliderComponent::Initialize()
{
	AttachToRigidbody(GetGameObject());
}

void Mage::BoxColliderComponent::RenderGizmos() const
{
	const auto wPos = GetGameObject()->GetTransform()->GetWorldPosition();
	const auto wSize = GetGameObject()->GetTransform()->GetWorldScale() * m_Size;
	const auto wRot = GetGameObject()->GetTransform()->GetWorldRotation() + m_Rotation;

	std::vector<glm::vec2> positions{
		{ -wSize.x / 2 + m_Offset.x, -wSize.y / 2 + m_Offset.y },
		{ -wSize.x / 2 + m_Offset.x, wSize.y / 2 + m_Offset.y },
		{ wSize.x / 2 + m_Offset.x, wSize.y / 2 + m_Offset.y },
		{ wSize.x / 2 + m_Offset.x, -wSize.y / 2 + m_Offset.y }
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
		bool isTrigger = IsTrigger();
		ImGuiHelper::ItemLabel("Is Trigger", ImGuiHelper::ItemLabelAlignment::Left);
		if(ImGui::Checkbox("##IsTrigger", &isTrigger))
            SetTrigger(isTrigger);

		glm::vec2 size = GetSize();
		ImGuiHelper::ItemLabel("Size", ImGuiHelper::ItemLabelAlignment::Left);
		if (ImGui::DragFloat2("##Size", &size.x, 0.1f))
            SetSize(size);

		glm::vec2 offset = GetOffset();
		ImGuiHelper::ItemLabel("Offset", ImGuiHelper::ItemLabelAlignment::Left);
	    if (ImGui::DragFloat2("##Offset", &offset.x, 0.1f))
            SetOffset(offset);

		float rotation = GetRotation();
		ImGuiHelper::ItemLabel("Rotation", ImGuiHelper::ItemLabelAlignment::Left);
		if (ImGui::DragFloat("##Rotation", &rotation))
            SetRotation(rotation);
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

const glm::vec2& Mage::BoxColliderComponent::GetSize() const
{
	return m_Size;
}

void Mage::BoxColliderComponent::SetSize(const glm::vec2& size)
{
    m_Size = size;
	RecalculateShape();
}

const glm::vec2& Mage::BoxColliderComponent::GetOffset() const
{
    return m_Offset;
}

void Mage::BoxColliderComponent::SetOffset(const glm::vec2& offset)
{
    m_Offset = offset;
	RecalculateShape();
}

float Mage::BoxColliderComponent::GetRotation() const
{
    return m_Rotation;
}

void Mage::BoxColliderComponent::SetRotation(float angle)
{
    m_Rotation = angle;
	RecalculateShape();
}

bool Mage::BoxColliderComponent::IsTrigger() const
{
    return m_RunTimeFixture->IsSensor();
}

void Mage::BoxColliderComponent::SetTrigger(bool isTrigger) const
{
    m_RunTimeFixture->SetSensor(isTrigger);
}

void Mage::BoxColliderComponent::RecalculateShape()
{
	m_InitialIsTrigger = IsTrigger();

	m_pRigidbody->RemoveBoxCollider(this);
	m_pRigidbody->AddBoxCollider(this, m_InitialIsTrigger);
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

void Mage::BoxColliderComponent::AttachToRigidbody(GameObject* gameObject)
{
    const auto rb = gameObject->GetComponentByType<RigidBodyComponent>();

    if (rb != nullptr)
    {
        rb->AddBoxCollider(this, m_InitialIsTrigger);
		m_pRigidbody = rb;
    }
    else
    {
		const auto parent = gameObject->GetParent();
		if (parent != nullptr)
		{
			AttachToRigidbody(parent);
		}
    }
}
