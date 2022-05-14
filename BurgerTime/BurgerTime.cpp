#include "BurgerTime/BurgerTimePCH.h"
#include "BurgerTime.h"

// Scenegraph
#include "Mage/Scenegraph/SceneManager.h"
#include "Mage/Scenegraph/Scene.h"
#include "Mage/Scenegraph/GameObject.h"

// Components
#include "Mage/Components/CameraComponent.h"
#include "Mage/Components/Transform.h"
#include "Mage/Components/SpriteComponent.h"
#include "Mage/Components/AnimatedSpriteComponent.h"
#include "Mage/Components/RigidBodyComponent.h"
#include "Mage/Components/BoxColliderComponent.h"
#include "Mage/Components/TilemapComponent.h"

#include "BurgerTime/PlayerAndEnemies/PlayerMovement.h"
#include "BurgerTime/PlayerAndEnemies/PeterPepper.h"
#include "BurgerTime/PlayerAndEnemies/EnemyMovement.h"
#include "BurgerTime/PlayerAndEnemies/Enemy.h"
#include "BurgerTime/Level.h"

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
	
	const auto cameraObject = scene->CreateChildObject("Camera");
	const auto camera = cameraObject->CreateComponent<Mage::CameraComponent>(glm::vec2{ 15.f, 15.f });
    Mage::ServiceLocator::GetRenderer()->SetCamera(camera);
	
	#pragma endregion

	// LEVEL
	//------
	#pragma region Level

	const auto levelObject = scene->CreateChildObject("Level");
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

	#pragma endregion

	// PETER PEPPER
	//-------------
	#pragma region PeterPepper
	
	const auto peterPepperObject = scene->CreateChildObject("PeterPepper");
	peterPepperObject->SetTag("PeterPepper");
	peterPepperObject->GetTransform()->SetLocalPosition({ 0, -4.3125f });

	peterPepperObject->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Dynamic, true, 0.f);
	peterPepperObject->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 0.5f, 0.5f }, glm::vec2{ 0.f, -0.25f }, 0.f, true);
	peterPepperObject->CreateComponent<Mage::AnimatedSpriteComponent>();
	peterPepperObject->CreateComponent<PlayerMovement>(level,
		std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("PeterPepper/Idle.png",      16), 1, 0.f),
		std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("PeterPepper/WalkFront.png", 16), 4, 0.1f),
		std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("PeterPepper/WalkBack.png",  16), 4, 0.1f),
		std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("PeterPepper/WalkLeft.png",  16), 4, 0.1f),
		std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("PeterPepper/WalkRight.png", 16), 4, 0.1f));
	peterPepperObject->CreateComponent<PeterPepper>(level,
		std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("PeterPepper/Victory.png", 16), 2, 0.2f),
		std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("PeterPepper/Death.png",   16), 6, 0.1f, false));
	
	#pragma endregion

	// MR HOT DOG
	//-----------
#pragma region MrHotDog

	const auto mrHotDogObject = scene->CreateChildObject("MrHotDog");
	mrHotDogObject->SetTag("Enemy");
	mrHotDogObject->GetTransform()->SetLocalPosition({ -6, 4.6875f });
	
	mrHotDogObject->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Dynamic, true, 0.f);
	mrHotDogObject->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 0.5f, 0.5f }, glm::vec2{ 0.f, -0.25f }, 0.f, true);
	mrHotDogObject->CreateComponent<Mage::AnimatedSpriteComponent>();
	const auto mrHotDogMovement = mrHotDogObject->CreateComponent<EnemyMovement>(level, peterPepperObject->GetTransform(),
		std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("MrHotDog/WalkFront.png", 16), 2, 0.1f),
		std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("MrHotDog/WalkBack.png",  16), 2, 0.1f),
		std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("MrHotDog/WalkLeft.png",  16), 2, 0.1f),
		std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("MrHotDog/WalkRight.png", 16), 2, 0.1f));
	mrHotDogObject->CreateComponent<Enemy>(mrHotDogMovement,
		std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("MrHotDog/Stunned.png", 16), 2, 0.1f),
		std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("MrHotDog/Death.png", 16), 4, 0.1f, false));

#pragma endregion
}
