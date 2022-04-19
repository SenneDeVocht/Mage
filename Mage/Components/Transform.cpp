#include "Mage/MagePCH.h"
#include "Transform.h"

#include "imgui.h"

#include "RigidBodyComponent.h"
#include "Mage/ImGui/ImGuiHelper.h"
#include "Mage/Scenegraph/GameObject.h"

void Mage::Transform::DrawProperties()
{
	Mage::ImGuiHelper::Component("Transform", this, nullptr, [&]()
	{
		if (ImGui::DragFloat2("Position", &m_Position.x, 0.1f))
		{
			ApplyTransformToRigidBodiesOfSelfAndChildren();
		}
	});
}

void Mage::Transform::SetPosition(const glm::vec2& position)
{
	m_Position = position;
	
	ApplyTransformToRigidBodiesOfSelfAndChildren();

	const auto rb = GetGameObject()->GetComponentByType<RigidBodyComponent>();
	if (rb != nullptr)
		rb->SetPosition(GetWorldPosition());
}

void Mage::Transform::SetWorldPosition(const glm::vec2& position)
{
	const auto parentWorldPos = GetWorldPosition() - m_Position;
	m_Position = position - parentWorldPos;

	ApplyTransformToRigidBodiesOfSelfAndChildren();
}

void Mage::Transform::Translate(const glm::vec2& translation)
{
	SetPosition(m_Position + translation);
}

glm::vec2 Mage::Transform::GetWorldPosition() const
{
	const auto gameObjectParent = m_pGameObject->GetParent();

	// No parent -> root object
	if (!gameObjectParent)
		return m_Position;

	return gameObjectParent->GetTransform()->GetWorldPosition() + m_Position;
}

void Mage::Transform::ApplyTransformToRigidBodiesOfSelfAndChildren() const
{
	// Self
	const auto rb = GetGameObject()->GetComponentByType<RigidBodyComponent>();
	if (rb != nullptr)
		rb->SetPosition(GetWorldPosition());

	// Children
	for (const auto child : GetGameObject()->GetChildren())
	{
		child->GetTransform()->ApplyTransformToRigidBodiesOfSelfAndChildren();
	}
}
