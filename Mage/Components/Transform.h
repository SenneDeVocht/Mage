#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
	class Transform final : public Component
	{
	public:
		const glm::vec2& GetLocalPosition() const { return m_Position; }
		glm::vec2 GetWorldPosition() const;

		void SetPosition(const glm::vec2& position);
		void Translate(const glm::vec2& translation);

		// Transform can't be destroyed
		void Destroy() override {}

	private:
		glm::vec2 m_Position;
	};
}
