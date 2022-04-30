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

		PhysicsHandler(const PhysicsHandler& other) = delete;
		PhysicsHandler(PhysicsHandler&& other) = delete;
		PhysicsHandler& operator=(const PhysicsHandler& other) = delete;
		PhysicsHandler& operator=(PhysicsHandler&& other) = delete;

		void UpdatePhysics() const;

		void AddRigidBody(RigidBodyComponent* rigidBody);
		void RemoveRigidBody(RigidBodyComponent* rigidBody);

		void AddBoxCollider(BoxColliderComponent* boxCollider) const;

		static int RigidBodyTypeToBox2D(int type);

	private:
		std::unique_ptr<b2World> m_pPhysicsWorld;
		std::unique_ptr<ContactListener> m_pContactListener;

		std::vector<RigidBodyComponent*> m_pRigidBodies;
	};
}
