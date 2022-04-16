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
			Dynamic,
			Kinematic
		};

		RigidBodyComponent(BodyType type, bool fixedRotation = true, float gravityScale = 1.0f);
		~RigidBodyComponent() = default;

		RigidBodyComponent(const RigidBodyComponent& other) = delete;
		RigidBodyComponent(RigidBodyComponent&& other) = delete;
		RigidBodyComponent& operator=(const RigidBodyComponent& other) = delete;
		RigidBodyComponent& operator=(RigidBodyComponent&& other) = delete;

		void Initialize() override;
		void FixedUpdate() override;

		BodyType GetType() const { return m_Type; }
		bool GetFixedRotation() const { return m_FixedRotation; }
		float GetGravityScale() const { return m_GravityScale; }

		void SetRunTimeBody(b2Body* body) { m_RunTimeBody = body; }
		b2Body* GetRunTimeBody() const { return m_RunTimeBody; }

		// Interaction
		void SetPosition(const glm::vec2& position);

		void SetVelocity(const glm::vec2& velocity);
		glm::vec2 GetVelocity() const;

		void ApplyForce(const glm::vec2& force, bool wake = true);
		void ApplyImpulse(const glm::vec2& impulse, bool wake = true);

	private:
		BodyType m_Type = BodyType::Static;
		bool m_FixedRotation = true;
		float m_GravityScale = 1.0f;

		// storage for runtime
		b2Body* m_RunTimeBody = nullptr;
	};
}
