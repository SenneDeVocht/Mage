#include "Mage/MagePCH.h"
#include "Transform.h"

#include "RigidBodyComponent.h"
#include "Mage/Scenegraph/GameObject.h"

void Mage::Transform::SetPosition(const glm::vec2& position)
{
	m_Position = position;

	// Special handling for rigid body component
	const auto rb = GetGameObject()->GetComponentByType<RigidBodyComponent>();
	if (rb != nullptr)
		rb->SetPosition(GetWorldPosition());
}

void Mage::Transform::SetWorldPosition(const glm::vec2& position)
{
	const auto parentWorldPos = GetWorldPosition() - m_Position;
	m_Position = position - parentWorldPos;

	// Special handling for rigid body component
	const auto rb = GetGameObject()->GetComponentByType<RigidBodyComponent>();
	if (rb != nullptr)
		rb->SetPosition(GetWorldPosition());
}

void Mage::Transform::Translate(const glm::vec2& translation)
{
	m_Position += translation;
}

glm::vec2 Mage::Transform::GetWorldPosition() const
{
	const auto gameObjectParent = m_pGameObject->GetParent();

	// No parent -> root object
	if (!gameObjectParent)
		return m_Position;

	return gameObjectParent->GetTransform()->GetWorldPosition() + m_Position;
}