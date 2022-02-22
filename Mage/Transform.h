#pragma once
#include "Component.h"

class Transform final : public Component
{
public:
	void Update() override {};
	void FixedUpdate() override {};
	void Render() const override {};

	const glm::vec3& GetLocalPosition() const { return m_Position; }
	glm::vec3 GetWorldPosition() const;
	void SetPosition(float x, float y, float z = 0);

private:
	glm::vec3 m_Position;
};