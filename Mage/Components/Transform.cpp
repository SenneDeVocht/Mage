#include "Mage/MagePCH.h"
#include "Transform.h"

#include "BoxColliderComponent.h"
#include "imgui.h"

#include "RigidBodyComponent.h"
#include "Mage/ImGui/ImGuiHelper.h"
#include "Mage/Scenegraph/GameObject.h"

void Mage::Transform::DrawProperties()
{
	Mage::ImGuiHelper::Component("Transform", this, nullptr, [&]()
	{
	    bool changed = false;

		ImGuiHelper::ItemLabel("Position", ImGuiHelper::ItemLabelAlignment::Left);
		changed |= ImGui::DragFloat2("##Position", &m_Position.x, 0.1f);

		ImGuiHelper::ItemLabel("Rotation", ImGuiHelper::ItemLabelAlignment::Left);
		changed |= ImGui::DragFloat("##Rotation", &m_Rotation, 0.1f);

		ImGuiHelper::ItemLabel("Scale", ImGuiHelper::ItemLabelAlignment::Left);
		changed |= ImGui::DragFloat2("##Scale", &m_Scale.x, 0.1f);

        if (changed)
			TransformChanged(true);
	});
}

glm::mat3x3 Mage::Transform::GetWorldMatrix()
{
    if (m_WorldMatrixDirty)
    {
		m_WorldMatrix =
		{
			cos(glm::radians(m_Rotation)) * m_Scale.x, sin(glm::radians(m_Rotation)) * m_Scale.x, 0.f,
			-sin(glm::radians(m_Rotation)) * m_Scale.y, cos(glm::radians(m_Rotation)) * m_Scale.y, 0.f,
            m_Position.x, m_Position.y, 1.f
		};

		const auto parent = GetGameObject()->GetParent();
        if (parent != nullptr)
        {
    		m_WorldMatrix = parent->GetTransform()->GetWorldMatrix() * m_WorldMatrix;
        }

        m_WorldMatrixDirty = false;
    }

	return m_WorldMatrix;
}

void Mage::Transform::TransformChanged(bool updateRigidBody)
{
	// Set world matrix dirty
	m_WorldMatrixDirty = true;

	// Update rigidbody
    if (updateRigidBody)
    {
		const auto rb = GetGameObject()->GetComponent<RigidBodyComponent>();
		if (rb != nullptr)
			rb->TransformChanged();

		const auto boxCollider = GetGameObject()->GetComponent<BoxColliderComponent>();
		if (boxCollider != nullptr && rb == nullptr)
			boxCollider->RecalculateShape();
    }

	// Update children
	for (const auto child : GetGameObject()->GetChildren())
	{
		child->GetTransform()->TransformChanged(updateRigidBody);
	}
}

void Mage::Transform::SetWorldPositionWithoutUpdatingRigidBody(const glm::vec2& position)
{
	const auto parent = GetGameObject()->GetParent();

	if (parent != nullptr)
	{
		// Avoid division by zero
		if (parent->GetTransform()->GetWorldScale().x == 0 ||
			parent->GetTransform()->GetWorldScale().y == 0)
		{
			m_Position = { 0, 0 };
		}
		else
		{
			const auto parentMatrix = parent->GetTransform()->GetWorldMatrix();
			m_Position = glm::inverse(parentMatrix) * glm::vec3(position.x, position.y, 1);
		}
	}
	else
	{
		m_Position = position;
	}

	TransformChanged(false);
}

void Mage::Transform::SetWorldRotationWithoutUpdatingRigidBody(float rotation)
{
	const auto parent = GetGameObject()->GetParent();

	if (parent != nullptr)
	{
		const float parentRotation = parent->GetTransform()->GetWorldRotation();

		m_Rotation = rotation - parentRotation;
	}
	else
	{
		m_Rotation = rotation;
	}

	TransformChanged(false);
}



// POSITION
//---------
glm::vec2 Mage::Transform::GetWorldPosition()
{
	const auto worldMatrix = GetWorldMatrix();
	return { worldMatrix[2].x, worldMatrix[2].y };
}

void Mage::Transform::SetLocalPosition(const glm::vec2& position)
{
	m_Position = position;
	TransformChanged(true);
}

void Mage::Transform::SetWorldPosition(const glm::vec2& position)
{
	const auto parent = GetGameObject()->GetParent();

	if (parent != nullptr)
	{
		// Avoid division by zero
		if (parent->GetTransform()->GetWorldScale().x == 0 ||
			parent->GetTransform()->GetWorldScale().y == 0)
		{
			m_Position = { 0, 0 };
		}
		else
		{
			const auto parentMatrix = parent->GetTransform()->GetWorldMatrix();
			m_Position = glm::inverse(parentMatrix) * glm::vec3(position.x, position.y, 1);
		}
	}
	else
	{
		m_Position = position;
	}

	TransformChanged(true);
}

void Mage::Transform::Translate(const glm::vec2& translation)
{
	SetLocalPosition(m_Position + translation);
}

// ROTATION
//---------
float Mage::Transform::GetWorldRotation()
{
	// Matrix decomposition from https://math.stackexchange.com/a/2888105

	const auto worldMatrix = GetWorldMatrix();
	const float a = worldMatrix[0].x;
	const float b = worldMatrix[0].y;
	const float c = worldMatrix[1].x;
	const float d = worldMatrix[1].y;

	if (a != 0 || b != 0) {
		const float r = sqrtf(powf(a, 2) + powf(b, 2));
		return glm::degrees(b > 0 ? acosf(a / r) : -acosf(a / r));
	}
	else if (c != 0 || d != 0) {
		const float s = sqrtf(powf(c, 2) + powf(d, 2));
		return 90 - glm::degrees(d > 0 ? acos(-c / s) : -acos(c / s));
	}

	return 0;
}

void Mage::Transform::SetLocalRotation(float rotation)
{
    m_Rotation = rotation;
	TransformChanged(true);
}

void Mage::Transform::SetWorldRotation(float rotation)
{
	const auto parent = GetGameObject()->GetParent();

	if (parent != nullptr)
	{
		const float parentRotation = parent->GetTransform()->GetWorldRotation();

		m_Rotation = rotation - parentRotation;
	}
	else
	{
		m_Rotation = rotation;
	}

	TransformChanged(true);
}

void Mage::Transform::Rotate(float rotation)
{
	SetLocalRotation(m_Rotation + rotation);
}

// SCALE
//------
glm::vec2 Mage::Transform::GetWorldScale()
{
    // Matrix decomposition from https://math.stackexchange.com/a/2888105

	const auto worldMatrix = GetWorldMatrix();
	const float a = worldMatrix[0].x;
	const float b = worldMatrix[0].y;
	const float c = worldMatrix[1].x;
	const float d = worldMatrix[1].y;
	const float delta = a * d - b * c;

	if (a != 0 || b != 0) {
		const float r = sqrtf(powf(a, 2) + powf(b, 2));
		return { r, delta / r };
	}
	else if (c != 0 || d != 0) {
		const float s = sqrtf(powf(c, 2) + powf(d, 2));
		return { delta / s, s };
	}
	else {
		return { 0, 0 };
	}
}

void Mage::Transform::SetLocalScale(const glm::vec2& scale)
{
    m_Scale = scale;
	TransformChanged(true);
}

void Mage::Transform::SetWorldScale(const glm::vec2& scale)
{
	const auto parent = GetGameObject()->GetParent();

	if (parent != nullptr)
	{
		const auto parentScale = parent->GetTransform()->GetWorldScale();

		m_Scale = scale / parentScale;
	}
	else
	{
		m_Scale = scale;
	}

	TransformChanged(true);
}

void Mage::Transform::Scale(const glm::vec2& factor)
{
	SetLocalScale(m_Scale * factor);
}