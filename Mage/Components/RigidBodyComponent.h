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

		void TransformChanged();
		void UpdateTransform() const;

		BodyType GetType() const { return m_Type; }
		bool GetFixedRotation() const { return m_FixedRotation; }
		float GetGravityScale() const { return m_GravityScale; }

		void SetRunTimeBody(b2Body* body) { m_RunTimeBody = body; }
		b2Body* GetRunTimeBody() const { return m_RunTimeBody; }

		// Interaction
		void SetVelocity(const glm::vec2& velocity) const;
		glm::vec2 GetVelocity() const;

		void ApplyForce(const glm::vec2& force, bool wake = true) const;
		void ApplyImpulse(const glm::vec2& impulse, bool wake = true) const;

	private:
		BodyType m_Type = BodyType::Static;
		bool m_FixedRotation = true;
		float m_GravityScale = 1.0f;

		// storage for runtime
		b2Body* m_RunTimeBody = nullptr;
	};
}
