#include "BurgerTime/BurgerTimePCH.h"
#include "BurgerTime.h"

// Scenegraph
#include "Mage/Scenegraph/SceneManager.h"
#include "Mage/Scenegraph/Scene.h"
#include "Mage/Scenegraph/GameObject.h"

// Components
#include "Mage/Components/SpriteComponent.h"
#include "Mage/Components/AnimatedSpriteComponent.h"
#include "Mage/Components/Transform.h"
#include "Mage/Components/CameraComponent.h"
#include "Mage/Components/RigidBodyComponent.h"
#include "Mage/Components/BoxColliderComponent.h"

#include "PeterPepper.h"
#include "Level.h"
#include "BurgerIngredient.h"

// Other
#include "Mage/Engine/Renderer.h"
#include "Mage/ResourceManagement/ResourceManager.h"

void BurgerTime::LoadGame() const
{
	const auto scene = Mage::SceneManager::GetInstance().CreateScene("DemoScene");

	// Camera
	//-------
	#pragma region Camera
	
	const auto cameraObject = scene->CreateObject("Camera");
	const auto camera = cameraObject->CreateComponent<Mage::CameraComponent>(16.f, 16.f);
	Mage::Renderer::GetInstance().SetCamera(camera);
	
	#pragma endregion

	// LEVEL
	//------
	#pragma region Level

	const auto levelParent = scene->CreateObject("Level");
	const auto level = levelParent->CreateComponent<Level>();
	level->LoadLevel();

	#pragma endregion

	// PETER PEPPER
	//-------------
	#pragma region PeterPepper
	
	const auto peterPepperObject = scene->CreateObject("PeterPepper");
	peterPepperObject->GetTransform()->SetPosition({ 0, -4.3125f });

	auto idle = peterPepperObject->CreateComponent<Mage::AnimatedSpriteComponent>(
		Mage::ResourceManager::GetInstance().LoadTexture("PeterPepper/Idle.png", 16),
		1,
		0.f);
	auto walkFront = peterPepperObject->CreateComponent<Mage::AnimatedSpriteComponent>(
		Mage::ResourceManager::GetInstance().LoadTexture("PeterPepper/WalkFront.png", 16),
		4,
		0.1f);
	auto walkBack = peterPepperObject->CreateComponent<Mage::AnimatedSpriteComponent>(
		Mage::ResourceManager::GetInstance().LoadTexture("PeterPepper/WalkBack.png", 16),
		4,
		0.1f);
	auto walkLeft = peterPepperObject->CreateComponent<Mage::AnimatedSpriteComponent>(
		Mage::ResourceManager::GetInstance().LoadTexture("PeterPepper/WalkLeft.png", 16),
		4,
		0.1f);
	auto walkRight = peterPepperObject->CreateComponent<Mage::AnimatedSpriteComponent>(
		Mage::ResourceManager::GetInstance().LoadTexture("PeterPepper/WalkRight.png", 16),
		4,
		0.1f);

	peterPepperObject->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Dynamic);
	peterPepperObject->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 0.5f, 0.5f }, glm::vec2{ 0.f, -0.25f }, true);
	peterPepperObject->CreateComponent<PeterPepper>(level, idle, walkFront, walkBack, walkLeft, walkRight);
	peterPepperObject->SetTag("PeterPepper");
	
	#pragma endregion

	// BURGER TEST
	//------------
	auto burger = scene->CreateObject("Burger");
	burger->GetTransform()->SetPosition({ -1.5f, 2 - 0.6875f });
	burger->CreateComponent<BurgerIngredient>(level, BurgerIngredient::IngredientType::BunTop);
	burger->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Dynamic);
	burger->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 2.f, 0.5f }, glm::vec2{ 0.f, 0.f }, true);
	burger->SetTag("Ingredient", true);

	burger = scene->CreateObject("Burger");
	burger->GetTransform()->SetPosition({ -1.5f, -0.6875f });
	burger->CreateComponent<BurgerIngredient>(level, BurgerIngredient::IngredientType::Patty);
	burger->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Dynamic);
	burger->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 2.f, 0.5f }, glm::vec2{ 0.f, 0.f }, true);
	burger->SetTag("Ingredient", true);

	burger = scene->CreateObject("Burger");
	burger->GetTransform()->SetPosition({ -1.5f, -2.6875f });
	burger->CreateComponent<BurgerIngredient>(level, BurgerIngredient::IngredientType::BunBottom);
	burger->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Dynamic);
	burger->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 2.f, 0.5f }, glm::vec2{ 0.f, 0.f }, true);
	burger->SetTag("Ingredient", true);

	// DONE
	//-----
	scene->Initialize();
}

void BurgerTime::CreatePlatform(Mage::GameObject* parent, const glm::vec2& position, bool isLight) const
{
	const std::string name = isLight ? "PlatformLight" : "PlatformDark";
	const auto object = parent->CreateChildObject(name);
	object->SetTag("Platform");

	object->GetTransform()->SetPosition(position);

	// Sprite
	const std::string texturePath = isLight ? "Level/Platform_Light.png" : "Level/Platform_Dark.png";
	object->CreateComponent<Mage::SpriteComponent>(Mage::ResourceManager::GetInstance().LoadTexture(texturePath, 16));

	// Triggerbox
	object->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Static);
	object->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{1, 0.125f}, glm::vec2{0, 0}, true);
}

void BurgerTime::CreateLadder(Mage::GameObject* parent, const glm::vec2& position) const
{
	const auto object = parent->CreateChildObject("Ladder");
	object->SetTag("Ladder");

	object->GetTransform()->SetPosition(position);

	// Sprite
	object->CreateComponent<Mage::SpriteComponent>(Mage::ResourceManager::GetInstance().LoadTexture("Level/Ladder.png", 16));

	// Triggerbox
	object->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Static);
	object->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 0.5f, 1.f }, glm::vec2{ 0, 0.25f }, true);
}