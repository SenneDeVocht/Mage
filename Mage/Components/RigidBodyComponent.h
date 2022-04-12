#pragma once
#include "Mage/Components/Component.h"

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

		RigidBodyComponent(BodyType type, bool fixedRotation = true);
		~RigidBodyComponent() = default;

		RigidBodyComponent(const RigidBodyComponent& other) = delete;
		RigidBodyComponent(RigidBodyComponent&& other) = delete;
		RigidBodyComponent& operator=(const RigidBodyComponent& other) = delete;
		RigidBodyComponent& operator=(RigidBodyComponent&& other) = delete;

		void Initialize() override;
		void FixedUpdate() override;

		BodyType GetType() const { return m_Type; }
		bool GetFixedRotation() const { return m_FixedRotation; }

		void SetRunTimeBody(void* body) { m_RunTimeBody = body; }
		void* GetRunTimeBody() const { return m_RunTimeBody; }

	private:
		BodyType m_Type = BodyType::Static;
		bool m_FixedRotation = true;

		// storage for runtime
		void* m_RunTimeBody = nullptr;
	};
}