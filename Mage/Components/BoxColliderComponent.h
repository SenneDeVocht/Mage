#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
	class BoxColliderComponent final : public Component
	{
	public:
		BoxColliderComponent();
		~BoxColliderComponent() = default;

		BoxColliderComponent(const BoxColliderComponent& other) = delete;
		BoxColliderComponent(BoxColliderComponent&& other) = delete;
		BoxColliderComponent& operator=(const BoxColliderComponent& other) = delete;
		BoxColliderComponent& operator=(BoxColliderComponent&& other) = delete;

		void Initialize() override;

		const glm::vec2& GetSize() const { return m_Size; }
		const glm::vec2& GetOffset() const { return m_Offset; }
		float GetDensity() const { return m_Density; }
		float GetFriction() const { return m_Friction; }
		float GetRestitution() const { return m_Restitution; }
		float GetRestitutionThreshold() const { return m_RestitutionThreshold; }

	private:
		glm::vec2 m_Offset = { 0.f, 0.f };
		glm::vec2 m_Size = { 0.5f, 0.5f };

		float m_Density = 1.f;
		float m_Friction = 0.5f;
		float m_Restitution = 0.f;
		float m_RestitutionThreshold = 0.5f;

		// storage for runtime
		void* m_RunTimeFixture = nullptr;
	};
}