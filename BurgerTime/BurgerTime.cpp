#include "BurgerTime/BurgerTimePCH.h"
#include "BurgerTime.h"

// Scenegraph
#include "Mage/Scenegraph/SceneManager.h"
#include "Mage/Scenegraph/Scene.h"
#include "Mage/Scenegraph/GameObject.h"

// Components
#include "Mage/Components/CameraComponent.h"
#include "Mage/Components/Transform.h"
#include "Mage/Components/AnimatedSpriteComponent.h"
#include "Mage/Components/TextComponent.h"
#include "Mage/Components/RigidBodyComponent.h"
#include "Mage/Components/BoxColliderComponent.h"
#include "Mage/Components/TilemapComponent.h"

#include "PeterPepper.h"
#include "Level.h"
#include "BurgerIngredient.h"
#include "SoundTester.h"

// Other
#include "Mage/Engine/ServiceLocator.h"
#include "Mage/Engine/Renderer.h"
#include "Mage/ResourceManagement/ResourceManager.h"

void BurgerTime::LoadGame() const
{
	const auto& resourceManager = Mage::ResourceManager::GetInstance();

    const auto scene = Mage::SceneManager::GetInstance().CreateScene("DemoScene");

	// Camera
	//-------
	#pragma region Camera
	
	const auto cameraObject = scene->CreateObject("Camera");
	const auto camera = cameraObject->CreateComponent<Mage::CameraComponent>(glm::vec2{ 15.f, 15.f });
    Mage::ServiceLocator::GetRenderer()->SetCamera(camera);
	
	#pragma endregion

	// LEVEL
	//------
	#pragma region Level

	const auto levelObject = scene->CreateObject("Level");
	levelObject->GetTransform()->SetWorldPosition({ 0.f, -0.5f });
	const auto level = levelObject->CreateComponent<Level>();
	levelObject->CreateComponent<Mage::TilemapComponent>(
		std::vector <std::shared_ptr<Mage::Texture2D>>{
		    resourceManager.LoadTexture("Level/Platform_Narrow.png", 16),
			resourceManager.LoadTexture("Level/Ladder_Narrow.png", 16),
			resourceManager.LoadTexture("Level/Ladder_And_Platform_Narrow.png", 16),
			resourceManager.LoadTexture("Level/Platform_Wide.png", 16),
			resourceManager.LoadTexture("Level/Ladder_Wide.png", 16),
			resourceManager.LoadTexture("Level/Ladder_And_Platform_Wide.png", 16)
		},
		glm::vec2{1.5f, 1.f});
	level->LoadLevel();

	#pragma endregion

	// PETER PEPPER
	//-------------
	#pragma region PeterPepper
	
	const auto peterPepperObject = scene->CreateObject("PeterPepper");
	peterPepperObject->GetTransform()->SetLocalPosition({ 0, -4.3125f });

	auto idle = peterPepperObject->CreateComponent<Mage::AnimatedSpriteComponent>(
		resourceManager.LoadTexture("PeterPepper/Idle.png", 16),
		1, 0.f, 1.f);
	auto walkFront = peterPepperObject->CreateComponent<Mage::AnimatedSpriteComponent>(
		resourceManager.LoadTexture("PeterPepper/WalkFront.png", 16),
		4, 0.1f, 1.f);
	auto walkBack = peterPepperObject->CreateComponent<Mage::AnimatedSpriteComponent>(
		resourceManager.LoadTexture("PeterPepper/WalkBack.png", 16),
		4, 0.1f, 1.f);
	auto walkLeft = peterPepperObject->CreateComponent<Mage::AnimatedSpriteComponent>(
		resourceManager.LoadTexture("PeterPepper/WalkLeft.png", 16),
		4, 0.1f, 1.f);
	auto walkRight = peterPepperObject->CreateComponent<Mage::AnimatedSpriteComponent>(
		resourceManager.LoadTexture("PeterPepper/WalkRight.png", 16),
		4, 0.1f, 1.f);

	peterPepperObject->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Dynamic, true, 0.f);
	peterPepperObject->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 0.5f, 0.5f }, glm::vec2{ 0.f, -0.25f }, 0.f, true);
	peterPepperObject->CreateComponent<PeterPepper>(level, idle, walkFront, walkBack, walkLeft, walkRight);
	peterPepperObject->SetTag("PeterPepper");
	
	#pragma endregion

	// BURGER TEST
	//------------
	auto burger = scene->CreateObject("Burger");
	burger->GetTransform()->SetLocalPosition({ -1.5f, 2 - 0.6875f });
	burger->CreateComponent<BurgerIngredient>(level, BurgerIngredient::IngredientType::BunTop);
	burger->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Dynamic, true, 0.f);
	burger->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 2.f, 0.5f }, glm::vec2{ 0.f, 0.f }, 0.f, true);
	burger->SetTag("Ingredient", true);

	burger = scene->CreateObject("Burger");
	burger->GetTransform()->SetLocalPosition({ -1.5f, -0.6875f });
	burger->CreateComponent<BurgerIngredient>(level, BurgerIngredient::IngredientType::Patty);
	burger->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Dynamic, true, 0.f);
	burger->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 2.f, 0.5f }, glm::vec2{ 0.f, 0.f }, 0.f, true);
	burger->SetTag("Ingredient", true);

	burger = scene->CreateObject("Burger");
	burger->GetTransform()->SetLocalPosition({ -1.5f, -2.6875f });
	burger->CreateComponent<BurgerIngredient>(level, BurgerIngredient::IngredientType::BunBottom);
	burger->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Dynamic, true, 0.f);
	burger->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 2.f, 0.5f }, glm::vec2{ 0.f, 0.f }, 0.f, true);
	burger->SetTag("Ingredient", true);

	// TEXT
	//-----
	const auto textObject = scene->CreateObject("Text");
	textObject->CreateComponent<Mage::TextComponent>("TEST", resourceManager.LoadFont("Cyber11.ttf", 11), SDL_Color{255, 255, 255, 255}, 16.f);

	// SOUND
	//------
	const auto sound = scene->CreateObject("Sound");
	sound->CreateComponent<SoundTester>();

	// DONE
	//-----
	scene->Initialize();
}