#pragma once
#include "Mage/Components/Component.h"

class b2Fixture;

namespace Mage
{
    class RigidBodyComponent;

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

		const glm::vec2& GetSize() const;
		void SetSize(const glm::vec2& size);

		const glm::vec2& GetOffset() const;
		void SetOffset(const glm::vec2& offset);

		float GetRotation() const;
		void SetRotation(float angle);

		bool IsTrigger() const;
		void SetTrigger(bool isTrigger) const;

	private:
		friend class PhysicsHandler;
		friend class RigidBodyComponent;
		friend class Transform;

		b2Fixture* GetRunTimeFixture() const { return m_RunTimeFixture; }
		void SetRunTimeFixture(b2Fixture* fixture) { m_RunTimeFixture = fixture; }

		void RecalculateShape();

		void NotifyGameObjectOnTriggerEnter(BoxColliderComponent* other) const;
		void NotifyGameObjectOnTriggerExit(BoxColliderComponent* other) const;
		void NotifyGameObjectOnCollisionEnter(BoxColliderComponent* other) const;
		void NotifyGameObjectOnCollisionExit(BoxColliderComponent* other) const;

		void AttachToRigidbody(GameObject* gameObject);

		// Collider shape
		glm::vec2 m_Size = { 1.f, 1.f };
		glm::vec2 m_Offset = { 0.f, 0.f };
		float m_Rotation = 0.f;

		bool m_InitialIsTrigger = false;

		RigidBodyComponent* m_pRigidbody = nullptr;

		// storage for runtime
		b2Fixture* m_RunTimeFixture = nullptr;
	};
}
