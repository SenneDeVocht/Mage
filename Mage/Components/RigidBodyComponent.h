#pragma once
#include "Mage/Components/Component.h"

class b2Body;

namespace Mage
{

	class RigidBodyComponent final : public Component
	{
	public:
		enum class BodyType
		{
			Static,
			Kinematic,
			Dynamic
		};

		RigidBodyComponent(BodyType type, bool fixedRotation = false, float gravityScale = 1.0f);
		~RigidBodyComponent();

		RigidBodyComponent(const RigidBodyComponent& other) = delete;
		RigidBodyComponent(RigidBodyComponent&& other) = delete;
		RigidBodyComponent& operator=(const RigidBodyComponent& other) = delete;
		RigidBodyComponent& operator=(RigidBodyComponent&& other) = delete;

		void Awake() override;
		void DrawProperties() override;

		void AddBoxCollider(BoxColliderComponent* boxCollider, bool isTrigger) const;
		void RemoveBoxCollider(BoxColliderComponent* boxCollider) const;

		void TransformChanged() const;
		void UpdateTransform() const;

		void SetRunTimeBody(b2Body* body) { m_RunTimeBody = body; }
		b2Body* GetRunTimeBody() const { return m_RunTimeBody; }


		BodyType GetType() const;
		void SetType(BodyType type) const;

		bool GetFixedRotation() const;
		void SetFixedRotation(bool fixedRotation) const;

		float GetGravityScale() const;
		void SetGravityScale(float gravityScale) const;

		glm::vec2 GetVelocity() const;
		void SetVelocity(const glm::vec2& velocity) const;

		void ApplyForce(const glm::vec2& force, bool wake = true) const;
		void ApplyImpulse(const glm::vec2& impulse, bool wake = true) const;

	private:
		BodyType m_InitialType;
		bool m_InitialFixedRotation;
		float m_InitialGravityScale;

		b2Body* m_RunTimeBody = nullptr;
	};
}
