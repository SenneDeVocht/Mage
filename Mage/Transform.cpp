#include "MagePCH.h"
#include "Transform.h"
#include "GameObject.h"

void Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

glm::vec3 Transform::GetWorldPosition() const
{
	auto gameObjectParent = m_pGameObject->GetParent();

	// No parent -> root object
	if (!gameObjectParent)
		return m_Position;

	return gameObjectParent->GetTransform()->GetWorldPosition() + m_Position;
}