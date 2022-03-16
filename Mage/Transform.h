#pragma once
#include "Component.h"

class Transform final : public Component
{
public:
	const glm::vec3& GetLocalPosition() const { return m_Position; }
	glm::vec3 GetWorldPosition() const;
	void SetPosition(float x, float y, float z = 0);

	// Transform can't be destroyed
	void Destroy() override{};

private:
	glm::vec3 m_Position;
};