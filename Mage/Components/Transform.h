#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
	class Transform final : public Component
	{
	public:
		void DrawProperties() override;

		const glm::vec2& GetLocalPosition() const { return m_Position; }
		glm::vec2 GetWorldPosition() const;

		void SetPosition(const glm::vec2& position);
		void SetWorldPosition(const glm::vec2& position);
		void Translate(const glm::vec2& translation);

		// Transform can't be destroyed
		void Destroy() override {}

	private:
		void ApplyTransformToRigidBodiesOfSelfAndChildren() const;

		glm::vec2 m_Position;
	};
}
