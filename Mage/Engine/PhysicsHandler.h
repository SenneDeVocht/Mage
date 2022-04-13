#pragma once

class b2World;

namespace Mage
{
	class ContactListener;
	class BoxColliderComponent;
	class RigidBodyComponent;

	class PhysicsHandler final
	{
	public:
		PhysicsHandler();
		~PhysicsHandler();

		void UpdatePhysics() const;

		void AddRigidBody(RigidBodyComponent* rigidBody) const;
		void AddBoxCollider(BoxColliderComponent* boxCollider) const;

	private:
		std::unique_ptr<b2World> m_pPhysicsWorld;
		std::unique_ptr<ContactListener> m_pContactListener;
	};
}
