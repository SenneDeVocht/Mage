#include "Mage/MagePCH.h"
#include "PhysicsHandler.h"

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/RigidBodyComponent.h"
#include "Mage/Components/BoxColliderComponent.h"
#include "Mage/Components/Transform.h"
#include "Mage/Engine/Timer.h"

#pragma warning(push, 0)        
#include <b2_world.h>
#include <b2_body.h>
#include <b2_polygon_shape.h>
#include <b2_fixture.h>
#include <b2_contact.h>
#pragma warning(pop)

namespace Mage
{
	class ContactListener : public b2ContactListener
	{
	public:

		void BeginContact(b2Contact* contact) override
		{
			// Get Colliders
			const auto colliderA = reinterpret_cast<Mage::BoxColliderComponent*>(contact->GetFixtureA()->GetUserData().pointer);
			const auto colliderB = reinterpret_cast<Mage::BoxColliderComponent*>(contact->GetFixtureB()->GetUserData().pointer);

			// Call correct functions
			if (colliderA->IsTrigger() || colliderB->IsTrigger())
			{
				colliderA->NotifyGameObjectOnTriggerEnter(colliderB);
				colliderB->NotifyGameObjectOnTriggerEnter(colliderA);
			}
			else
			{
				colliderA->NotifyGameObjectOnCollisionEnter(colliderB);
				colliderB->NotifyGameObjectOnCollisionEnter(colliderA);
			}
		}

		void EndContact(b2Contact* contact) override
		{
			// Get Colliders
			const auto colliderA = reinterpret_cast<Mage::BoxColliderComponent*>(contact->GetFixtureA()->GetUserData().pointer);
			const auto colliderB = reinterpret_cast<Mage::BoxColliderComponent*>(contact->GetFixtureB()->GetUserData().pointer);

			// Call correct functions
			if (colliderA->IsTrigger() || colliderB->IsTrigger())
			{
				colliderA->NotifyGameObjectOnTriggerExit(colliderB);
				colliderB->NotifyGameObjectOnTriggerExit(colliderA);
			}
			else
			{
				colliderA->NotifyGameObjectOnCollisionExit(colliderB);
				colliderB->NotifyGameObjectOnCollisionExit(colliderA);
			}
		}

		//void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override
		//{
		//	
		//}
		//
		//void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override
		//{
		//	
		//}
	};
}

Mage::PhysicsHandler::PhysicsHandler()
	: m_pPhysicsWorld{ std::make_unique<b2World>(b2Vec2(0.0f, -9.81f)) }
	, m_pContactListener{ std::make_unique<ContactListener>() }
{
	m_pPhysicsWorld->SetContactListener(m_pContactListener.get());
}

Mage::PhysicsHandler::~PhysicsHandler()
{}

void Mage::PhysicsHandler::UpdatePhysics() const
{
	constexpr int32_t velocityIterations = 6;
	constexpr int32_t positionIterations = 2;

	const float timeStep = Mage::Timer::GetInstance().GetFixedTimeStep();
	m_pPhysicsWorld->Step(timeStep, velocityIterations, positionIterations);
}

b2BodyType RigidBodyTypeToBox2D(Mage::RigidBodyComponent::BodyType type)
{
	switch (type)
	{
	case Mage::RigidBodyComponent::BodyType::Static:
		return b2_staticBody;

	case Mage::RigidBodyComponent::BodyType::Dynamic:
		return b2_dynamicBody;

	case Mage::RigidBodyComponent::BodyType::Kinematic:
		return b2_kinematicBody;
	}

	return b2_staticBody;
}

void Mage::PhysicsHandler::AddRigidBody(RigidBodyComponent* rigidBody) const
{
	const auto transform = rigidBody->GetGameObject()->GetTransform();

	b2BodyDef bodyDef;
	bodyDef.type = RigidBodyTypeToBox2D(rigidBody->GetType());
	bodyDef.position.Set(
		transform->GetWorldPosition().x,
		transform->GetWorldPosition().y);
	bodyDef.gravityScale = rigidBody->GetGravityScale();
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(rigidBody);

	const auto body = m_pPhysicsWorld->CreateBody(&bodyDef);
	body->SetFixedRotation(rigidBody->GetFixedRotation());

	rigidBody->SetRunTimeBody(body);
}

void Mage::PhysicsHandler::AddBoxCollider(BoxColliderComponent* boxCollider) const
{
	const auto rigidBody = boxCollider->GetGameObject()->GetComponentByType<RigidBodyComponent>();
	assert(rigidBody != nullptr);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(
		boxCollider->GetSize().x / 2.f, boxCollider->GetSize().y / 2.f,
		{ boxCollider->GetOffset().x, boxCollider->GetOffset().y },
		0.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = boxCollider->GetDensity();
	fixtureDef.friction = boxCollider->GetFriction();
	fixtureDef.restitution = boxCollider->GetRestitution();
	fixtureDef.restitutionThreshold = boxCollider->GetRestitutionThreshold();
	fixtureDef.isSensor = boxCollider->IsTrigger();
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(boxCollider);

	const auto fixture = rigidBody->GetRunTimeBody()->CreateFixture(&fixtureDef);

	boxCollider->SetRunTimeFixture(fixture);
}