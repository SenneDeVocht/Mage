#include "Mage/MagePCH.h"
#include "Transform.h"

#include "Mage/Scenegraph/GameObject.h"

void Mage::Transform::SetPosition(const glm::vec2& position)
{
	m_Position = position;
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