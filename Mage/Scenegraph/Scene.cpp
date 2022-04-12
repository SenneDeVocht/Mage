#include "Mage/MagePCH.h"
#include "Scene.h"

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Engine/Timer.h"

#include "Mage/Components/RigidBodyComponent.h"
#include "Mage/Components/BoxColliderComponent.h"
#include "Mage/Components/Transform.h"
#include "b2_world.h"
#include "b2_body.h"
#include "b2_fixture.h"
#include "b2_polygon_shape.h"

unsigned int Mage::Scene::m_IdCounter = 0;

Mage::Scene::Scene(const std::string& name)
	: m_Name(name)
	, m_pPhysicsWorld{ std::make_unique<b2World>(b2Vec2(0.0f, -9.81f)) }
{}

Mage::Scene::~Scene()
{}

const std::string& Mage::Scene::GetName() const
{
	return m_Name;
}

Mage::GameObject* Mage::Scene::CreateObject(const std::string& name)
{
	auto object = std::unique_ptr<GameObject>(new GameObject(name, nullptr, this));
	const auto pObject = object.get();

	m_Objects.push_back(std::move(object));

	return pObject;
}

std::vector<Mage::GameObject*> Mage::Scene::GetObjects() const
{
	std::vector<GameObject*> vecRaw;

	vecRaw.reserve(m_Objects.size());
	std::transform(m_Objects.cbegin(), m_Objects.cend(), std::back_inserter(vecRaw),
		[](auto& ptr) { return ptr.get(); });

	return vecRaw;
}

static b2BodyType RigidBodyTypeToBox2D(Mage::RigidBodyComponent::BodyType type)
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

void Mage::Scene::AddRigidBody(RigidBodyComponent* rigidBody)
{
	const auto transform = rigidBody->GetGameObject()->GetTransform();

	b2BodyDef bodyDef;
	bodyDef.type = RigidBodyTypeToBox2D(rigidBody->GetType());
	bodyDef.position.Set(
		transform->GetWorldPosition().x,
		transform->GetWorldPosition().y);

	b2Body* body = m_pPhysicsWorld->CreateBody(&bodyDef);
	body->SetFixedRotation(rigidBody->GetFixedRotation());

	rigidBody->SetRunTimeBody(body);
}

void Mage::Scene::AddBoxCollider(BoxColliderComponent* boxCollider)
{
	auto rigidBody = boxCollider->GetGameObject()->GetComponentByType<RigidBodyComponent>();
	assert(rigidBody != nullptr);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(
		boxCollider->GetSize().x, boxCollider->GetSize().y,
		{ boxCollider->GetOffset().x, boxCollider->GetOffset().y },
		0.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = boxCollider->GetDensity();
	fixtureDef.friction = boxCollider->GetFriction();
	fixtureDef.restitution = boxCollider->GetRestitution();
	fixtureDef.restitutionThreshold = boxCollider->GetRestitutionThreshold();

	static_cast<b2Body*>(rigidBody->GetRunTimeBody())->CreateFixture(&fixtureDef);
}

void Mage::Scene::DrawImGui() const
{
	for (const auto& object : m_Objects)
	{
		object->DrawImGui();
	}
}

void Mage::Scene::Update() const
{
	for (const auto& object : m_Objects)
	{
		object->Update();
	}
}

void Mage::Scene::FixedUpdate() const
{
	// Physics
	const int32_t velocityIterations = 6;
	const int32_t positionIterations = 2;

	const float timeStep = Mage::Timer::GetInstance().GetFixedTimeStep();
	m_pPhysicsWorld->Step(timeStep, velocityIterations, positionIterations);


	// Fixed Update on objects
	for (const auto& object : m_Objects)
	{
		object->FixedUpdate();
	}
}

void Mage::Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

void Mage::Scene::ChangeSceneGraph()
{
	// Destroy marked root objects
	const auto pos = std::remove_if(m_Objects.begin(), m_Objects.end(),
		[](const auto& o) { return o->IsMarkedForDestroy(); });

	m_Objects.erase(pos, m_Objects.end());

	// Call on remaining objects
	for (const auto& object : m_Objects)
	{
		object->ChangeSceneGraph();
	}
}