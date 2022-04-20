#include "BurgerTime/BurgerTimePCH.h"
#include "BurgerIngredient.h"

#include "Mage/Engine/Timer.h"
#include "Mage/ResourceManagement/ResourceManager.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/Transform.h"
#include "Mage/Components/SpriteComponent.h"
#include "Mage/Components/RigidBodyComponent.h"
#include "Mage/Components/BoxColliderComponent.h"

#include "BurgerIngredientPart.h"
#include "Level.h"

BurgerIngredient::BurgerIngredient(Level* level, IngredientType type)
	: m_pLevel{ level }
	, m_Type{ type }
{}

void BurgerIngredient::Initialize()
{
	std::string name;
	switch (m_Type)
	{
		case IngredientType::BunTop:
			name = "Bun_Top";
			break;
		case IngredientType::BunBottom:
			name = "Bun_Bottom";
			break;
		case IngredientType::Cheese:
			name = "Cheese";
			break;
		case IngredientType::Patty:
			name = "Patty";
			break;
		case IngredientType::Tomato:
			name = "Tomato";
			break;
		case IngredientType::Salad:
			name = "Salad";
			break;
	}

	// LEFT
	auto go = GetGameObject()->CreateChildObject("Left");
	go->GetTransform()->SetLocalPosition({ -0.75f, 0 });
	go->CreateComponent<Mage::SpriteComponent>(Mage::ResourceManager::GetInstance().LoadTexture("Ingredients/" + name + "/Left.png", 16));
	go->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Kinematic);
	go->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{0.5f, 0.5f}, glm::vec2{0, 0}, 0.f, true);
	m_Parts.push_back(go->CreateComponent<BurgerIngredientPart>(this));

	// MID LEFT
	go = GetGameObject()->CreateChildObject("MidLeft");
	go->GetTransform()->SetLocalPosition({ -0.25f, 0 });
	go->CreateComponent<Mage::SpriteComponent>(Mage::ResourceManager::GetInstance().LoadTexture("Ingredients/" + name + "/Mid_Left.png", 16));
	go->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Kinematic);
	go->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 0.5f, 0.5f }, glm::vec2{ 0, 0 }, 0.f, true);
	m_Parts.push_back(go->CreateComponent<BurgerIngredientPart>(this));
	
	// MID RIGHT
	go = GetGameObject()->CreateChildObject("MidRight");
	go->GetTransform()->SetLocalPosition({ 0.25f, 0 });
	go->CreateComponent<Mage::SpriteComponent>(Mage::ResourceManager::GetInstance().LoadTexture("Ingredients/" + name + "/Mid_Right.png", 16));
	go->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Kinematic);
	go->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 0.5f, 0.5f }, glm::vec2{ 0, 0 }, 0.f, true);
	m_Parts.push_back(go->CreateComponent<BurgerIngredientPart>(this));
    
	// RIGHT
	go = GetGameObject()->CreateChildObject("Right");
	go->GetTransform()->SetLocalPosition({ 0.75f, 0 });
	go->CreateComponent<Mage::SpriteComponent>(Mage::ResourceManager::GetInstance().LoadTexture("Ingredients/" + name + "/Right.png", 16));
	go->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Kinematic);
	go->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 0.5f, 0.5f }, glm::vec2{ 0, 0 }, 0.f, true);
	m_Parts.push_back(go->CreateComponent<BurgerIngredientPart>(this));
}

void BurgerIngredient::Update()
{
	if (m_Falling)
	{
		const float deltaTime = Mage::Timer::GetInstance().GetDeltaTime();
		m_FallVelocity += m_FallAcceleration * deltaTime;

		const auto rigidBody = GetGameObject()->GetComponentByType<Mage::RigidBodyComponent>();
		rigidBody->SetVelocity(m_FallVelocity);

		// Destination reached
		const auto transform = GetGameObject()->GetTransform();
		if (transform->GetWorldPosition().y <= m_FallDestination)
		{
			rigidBody->SetVelocity({ 0, 0 });

			transform->SetLocalPosition({ transform->GetWorldPosition().x, m_FallDestination });
			m_FallVelocity = {0, 0};
			m_Falling = false;

			// Reset parts
			m_PartsSteppedOn = 0;

			for (auto part : m_Parts)
			{
				part->DoneFalling();
			}
		}
	}
}


void BurgerIngredient::PartSteppedOn()
{
	++m_PartsSteppedOn;

	// Start falling
	if (m_PartsSteppedOn >= (int)m_Parts.size())
	{
		for (auto part : m_Parts)
		{
			part->ReadyForFall();
		}

		const auto transform = GetGameObject()->GetTransform();
		m_FallDestination = m_pLevel->GetNextPlatformDown(transform->GetWorldPosition()).y - 3 / 16.f;
		m_Falling = true;
	}
}