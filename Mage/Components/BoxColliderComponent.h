#pragma once
#include "Mage/Components/Component.h"

class b2Fixture;

namespace Mage
{
	class BoxColliderComponent final : public Component
	{
	public:
		BoxColliderComponent(const glm::vec2& size, const glm::vec2& offset, float angle, bool isTrigger = false);
		~BoxColliderComponent() override = default;

		BoxColliderComponent(const BoxColliderComponent& other) = delete;
		BoxColliderComponent(BoxColliderComponent&& other) = delete;
		BoxColliderComponent& operator=(const BoxColliderComponent& other) = delete;
		BoxColliderComponent& operator=(BoxColliderComponent&& other) = delete;

		void Initialize() override;
		void RenderGizmos() const override;
		void DrawProperties() override;

		void SetEnabled(bool enabled) override;

		const glm::vec2& GetSize() const { return m_Size; }
		const glm::vec2& GetOffset() const { return m_Offset; }
		float GetAngle() const { return m_Angle; }
		bool IsTrigger() const { return m_IsTrigger; }

		float GetDensity() const { return m_Density; }
		float GetFriction() const { return m_Friction; }
		float GetRestitution() const { return m_Restitution; }
		float GetRestitutionThreshold() const { return m_RestitutionThreshold; }

		void RecalculateShape();
		void SetRunTimeFixture(b2Fixture* fixture){ m_RunTimeFixture = fixture; }

		void NotifyGameObjectOnTriggerEnter(BoxColliderComponent* other) const;
		void NotifyGameObjectOnTriggerExit(BoxColliderComponent* other) const;
		void NotifyGameObjectOnCollisionEnter(BoxColliderComponent* other) const;
		void NotifyGameObjectOnCollisionExit(BoxColliderComponent* other) const;

	private:
		// Collider
		glm::vec2 m_Size = { 1.f, 1.f };
		glm::vec2 m_Offset = { 0.f, 0.f };
		float m_Angle = 0.f;
		bool m_IsTrigger = false;

		// TODO: Physics Material
		float m_Density = 1.f;
		float m_Friction = 0.5f;
		float m_Restitution = 0.5f;
		float m_RestitutionThreshold = 0.5f;

		// storage for runtime
		b2Fixture* m_RunTimeFixture = nullptr;

		// Needed to see if new shape needs to be created
		glm::vec2 m_PreviousSize = { 0.f, 0.f };
		glm::vec2 m_PreviousOffset = { 0.f, 0.f };
		float m_PreviousAngle = 0.f;
	};
}
