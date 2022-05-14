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
#include <b2_fixture.h>
#include <b2_contact.h>
#pragma warning(pop)

namespace Mage
{
	class ContactListener : public b2ContactListener
	{
	public:
		enum class ContacType
		{
			Begin, End
		};

		struct Contact
		{
			BoxColliderComponent* ColliderA;
			BoxColliderComponent* ColliderB;
			ContacType Type;
		};

		void BeginContact(b2Contact* contact) override
		{
			// Get Colliders
			const auto colliderA = reinterpret_cast<BoxColliderComponent*>(contact->GetFixtureA()->GetUserData().pointer);
			const auto colliderB = reinterpret_cast<BoxColliderComponent*>(contact->GetFixtureB()->GetUserData().pointer);

			// Store contact for later use
			m_Contacts.emplace_back(Contact{ colliderA, colliderB, ContacType::Begin });
		}

		void EndContact(b2Contact* contact) override
		{
			// Get Colliders
			const auto colliderA = reinterpret_cast<Mage::BoxColliderComponent*>(contact->GetFixtureA()->GetUserData().pointer);
			const auto colliderB = reinterpret_cast<Mage::BoxColliderComponent*>(contact->GetFixtureB()->GetUserData().pointer);

			// Store contact for later use
			m_Contacts.emplace_back(Contact{ colliderA, colliderB, ContacType::End });
		}
		
		const std::vector<Contact> GetContacts() const { return m_Contacts; }
		void ResetContacts() { m_Contacts.clear(); }

	private:
		std::vector<Contact> m_Contacts;
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
	// Reset
	m_pContactListener->ResetContacts();

	// Take physics steps
	constexpr int32_t velocityIterations = 6;
	constexpr int32_t positionIterations = 2;

	const float timeStep = Mage::Timer::GetInstance().GetFixedTimeStep();
	m_pPhysicsWorld->Step(timeStep, velocityIterations, positionIterations);

	// Update transforms
	for (auto rb : m_pRigidBodies)
	{
		rb->UpdateTransform();
	}

	// Call collision callbacks
	for (auto& contact : m_pContactListener->GetContacts())
	{
		if (contact.Type == ContactListener::ContacType::Begin)
		{
			if (contact.ColliderA->IsTrigger() || contact.ColliderB->IsTrigger())
			{
				contact.ColliderA->NotifyGameObjectOnTriggerEnter(contact.ColliderB);
				contact.ColliderB->NotifyGameObjectOnTriggerEnter(contact.ColliderA);
			}
			else
			{
				contact.ColliderA->NotifyGameObjectOnCollisionEnter(contact.ColliderB);
				contact.ColliderB->NotifyGameObjectOnCollisionEnter(contact.ColliderA);
			}
		}
		else if(contact.Type == ContactListener::ContacType::End)
		{
			if (contact.ColliderA->IsTrigger() || contact.ColliderB->IsTrigger())
			{
				contact.ColliderA->NotifyGameObjectOnTriggerExit(contact.ColliderB);
				contact.ColliderB->NotifyGameObjectOnTriggerExit(contact.ColliderA);
			}
			else
			{
				contact.ColliderA->NotifyGameObjectOnCollisionExit(contact.ColliderB);
				contact.ColliderB->NotifyGameObjectOnCollisionExit(contact.ColliderA);
			}
		}
	}
}

void Mage::PhysicsHandler::AddRigidBody(RigidBodyComponent* rigidBody, int type, bool fixedRotation, float gravityScale)
{
	// Store pointer
	m_pRigidBodies.push_back(rigidBody);

	// Add to physics world
	const auto transform = rigidBody->GetGameObject()->GetTransform();

	b2BodyDef bodyDef;
	bodyDef.type = static_cast<b2BodyType>(type);
	bodyDef.position.Set(
		transform->GetWorldPosition().x,
		transform->GetWorldPosition().y);
	bodyDef.gravityScale = gravityScale;
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(rigidBody);

	const auto body = m_pPhysicsWorld->CreateBody(&bodyDef);
	body->SetFixedRotation(fixedRotation);

	rigidBody->SetRunTimeBody(body);
}

void Mage::PhysicsHandler::RemoveRigidBody(RigidBodyComponent* rigidBody)
{
	// Remove from physics world
	m_pPhysicsWorld->DestroyBody(rigidBody->GetRunTimeBody());

	// Remove from component vector
	const auto pos = std::remove(m_pRigidBodies.begin(), m_pRigidBodies.end(), rigidBody);
	m_pRigidBodies.erase(pos, m_pRigidBodies.end());
}
