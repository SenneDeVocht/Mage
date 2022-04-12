#include "BurgerTime/BurgerTimePCH.h"
#include "BurgerTime.h"

// Scenegraph
#include "Mage/Scenegraph/SceneManager.h"
#include "Mage/Scenegraph/Scene.h"
#include "Mage/Scenegraph/GameObject.h"

// Components
#include "Mage/Components/SpriteComponent.h"
#include "Mage/Components/AnimatedSpriteComponent.h"
#include "Mage/Components/TextComponent.h"
#include "Mage/Components/Transform.h"
#include "Mage/Components/CameraComponent.h"
#include "Mage/Components/RigidBodyComponent.h"
#include "Mage/Components/BoxColliderComponent.h"
#include "PeterPepper.h"

// Other
#include "Mage/Engine/Renderer.h"
#include "Mage/Input/Command.h"
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

	// LADDERS
	//--------
	#pragma region Ladders
	// First collumn
	CreateLadder(levelParent, { -6, 4.25f });
	CreateLadder(levelParent, { -6, 3.25f });
	CreateLadder(levelParent, { -6, 0.25f });
	CreateLadder(levelParent, { -6, -0.75f });
	CreateLadder(levelParent, { -6, -1.75f });
	CreateLadder(levelParent, { -6, -2.75f });
	CreateLadder(levelParent, { -6, -3.75f });

	// Second collumn
	CreateLadder(levelParent, { -4.5f, 2.25f });
	CreateLadder(levelParent, { -4.5f, 1.25f });
	CreateLadder(levelParent, { -4.5f, 0.25f });
	CreateLadder(levelParent, { -4.5f, -0.75f });
	CreateLadder(levelParent, { -4.5f, -1.75f });

	// Third collumn
	CreateLadder(levelParent, { -3, 4.25f });
	CreateLadder(levelParent, { -3, 3.25f });
	CreateLadder(levelParent, { -3, 2.25f });
	CreateLadder(levelParent, { -3, 1.25f });
	CreateLadder(levelParent, { -3, 0.25f });
	CreateLadder(levelParent, { -3, -0.75f });
	CreateLadder(levelParent, { -3, -1.75f });
	CreateLadder(levelParent, { -3, -2.75f });
	CreateLadder(levelParent, { -3, -3.75f });

	// Fourth collumn
	CreateLadder(levelParent, { -1.5f, 4.25f });
	CreateLadder(levelParent, { -1.5f, 3.25f });
	CreateLadder(levelParent, { -1.5f, 2.25f });

	// Fifth collumn
	CreateLadder(levelParent, { 0, 4.25f });
	CreateLadder(levelParent, { 0, 3.25f });
	CreateLadder(levelParent, { 0, 2.25f });
	CreateLadder(levelParent, { 0, 1.25f });
	CreateLadder(levelParent, { 0, 0.25f });
	CreateLadder(levelParent, { 0, -0.75f });
	CreateLadder(levelParent, { 0, -1.75f });
	CreateLadder(levelParent, { 0, -2.75f });
	CreateLadder(levelParent, { 0, -3.75f });

	// Sixth collumn
	CreateLadder(levelParent, { 1.5f, 2.25f });
	CreateLadder(levelParent, { 1.5f, 1.25f });
	CreateLadder(levelParent, { 1.5f, 0.25f });

	// Seventh collumn
	CreateLadder(levelParent, { 3, 4.25f });
	CreateLadder(levelParent, { 3, 3.25f });
	CreateLadder(levelParent, { 3, 2.25f });
	CreateLadder(levelParent, { 3, 1.25f });
	CreateLadder(levelParent, { 3, 0.25f });
	CreateLadder(levelParent, { 3, -0.75f });
	CreateLadder(levelParent, { 3, -1.75f });
	CreateLadder(levelParent, { 3, -2.75f });
	CreateLadder(levelParent, { 3, -3.75f });

	// Eighth collumn
	CreateLadder(levelParent, { 4.5f, 0.25f });
	CreateLadder(levelParent, { 4.5f, -0.75f });
	CreateLadder(levelParent, { 4.5f, -1.75f });
	CreateLadder(levelParent, { 4.5f, -2.75f });
	CreateLadder(levelParent, { 4.5f, -3.75f });

	// Ninth collumn
	CreateLadder(levelParent, { 6, 4.25f });
	CreateLadder(levelParent, { 6, 3.25f });
	CreateLadder(levelParent, { 6, 2.25f });
	CreateLadder(levelParent, { 6, 1.25f });
	CreateLadder(levelParent, { 6, -1.75f });
	CreateLadder(levelParent, { 6, -2.75f });
	CreateLadder(levelParent, { 6, -3.75f });
	#pragma endregion

	// PLATFORMS
	//----------
	#pragma region Platforms
	// First row
	CreatePlatform(levelParent, {-6, 5}, true);
	CreatePlatform(levelParent, {-5, 5});
	CreatePlatform(levelParent, {-4, 5});
	CreatePlatform(levelParent, {-3, 5}, true);
	CreatePlatform(levelParent, {-2, 5});
	CreatePlatform(levelParent, {-1, 5});
	CreatePlatform(levelParent, {0, 5}, true);
	CreatePlatform(levelParent, { 1, 5 });
	CreatePlatform(levelParent, { 2, 5 });
	CreatePlatform(levelParent, { 3, 5 }, true);
	CreatePlatform(levelParent, { 4, 5 });
	CreatePlatform(levelParent, { 5, 5 });
	CreatePlatform(levelParent, { 6, 5 }, true);

	// Second row
	CreatePlatform(levelParent, { -6, 3 }, true);
	CreatePlatform(levelParent, { -5, 3 });
	CreatePlatform(levelParent, { -4, 3 });
	CreatePlatform(levelParent, { -3, 3 }, true);
	CreatePlatform(levelParent, { 0, 3 }, true);
	CreatePlatform(levelParent, { 1, 3 });
	CreatePlatform(levelParent, { 2, 3 });
	CreatePlatform(levelParent, { 3, 3 }, true);
	CreatePlatform(levelParent, { 4, 3 });
	CreatePlatform(levelParent, { 5, 3 });
	CreatePlatform(levelParent, { 6, 3 }, true);

	// Third row
	CreatePlatform(levelParent, { -3, 2 }, true);
	CreatePlatform(levelParent, { -2, 2 });
	CreatePlatform(levelParent, { -1, 2 });
	CreatePlatform(levelParent, { 0, 2 }, true);

	// Fourth row
	CreatePlatform(levelParent, { -6, 1 }, true);
	CreatePlatform(levelParent, { -5, 1 });
	CreatePlatform(levelParent, { -4, 1 });
	CreatePlatform(levelParent, { -3, 1 }, true);
	CreatePlatform(levelParent, { 3, 1 }, true);
	CreatePlatform(levelParent, { 4, 1 });
	CreatePlatform(levelParent, { 5, 1 });
	CreatePlatform(levelParent, { 6, 1 }, true);

	// Fifth row
	CreatePlatform(levelParent, { -3, 0 }, true);
	CreatePlatform(levelParent, { -2, 0 });
	CreatePlatform(levelParent, { -1, 0 });
	CreatePlatform(levelParent, { 0, 0 }, true);
	CreatePlatform(levelParent, { 1, 0 });
	CreatePlatform(levelParent, { 2, 0 });
	CreatePlatform(levelParent, { 3, 0 }, true);

	// Sixth row
	CreatePlatform(levelParent, { 3, -1 }, true);
	CreatePlatform(levelParent, { 4, -1 });
	CreatePlatform(levelParent, { 5, -1 });
	CreatePlatform(levelParent, { 6, -1 }, true);

	// Seventh row
	CreatePlatform(levelParent, { -6, -2 }, true);
	CreatePlatform(levelParent, { -5, -2 });
	CreatePlatform(levelParent, { -4, -2 });
	CreatePlatform(levelParent, { -3, -2 }, true);
	CreatePlatform(levelParent, { -2, -2 });
	CreatePlatform(levelParent, { -1, -2 });
	CreatePlatform(levelParent, { 0, -2 }, true);
	CreatePlatform(levelParent, { 1, -2 });
	CreatePlatform(levelParent, { 2, -2 });
	CreatePlatform(levelParent, { 3, -2 }, true);

	// Eighth row
	CreatePlatform(levelParent, { -6, -4 }, true);
	CreatePlatform(levelParent, { -5, -4 });
	CreatePlatform(levelParent, { -4, -4 });
	CreatePlatform(levelParent, { -3, -4 }, true);
	CreatePlatform(levelParent, { -2, -4 });
	CreatePlatform(levelParent, { -1, -4 });
	CreatePlatform(levelParent, { 0, -4 }, true);
	CreatePlatform(levelParent, { 1, -4 });
	CreatePlatform(levelParent, { 2, -4 });
	CreatePlatform(levelParent, { 3, -4 }, true);
	CreatePlatform(levelParent, { 4, -4 });
	CreatePlatform(levelParent, { 5, -4 });
	CreatePlatform(levelParent, { 6, -4 }, true);
	#pragma endregion

	#pragma endregion

	// PETER PEPPER
	//-------------
	#pragma region PeterPepper
	
	const auto peterPepperObject = scene->CreateObject("PeterPepper");

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

	peterPepperObject->CreateComponent<PeterPepper>(idle, walkFront, walkBack, walkLeft, walkRight);

	peterPepperObject->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Dynamic);
	peterPepperObject->CreateComponent<Mage::BoxColliderComponent>();
	
	#pragma endregion
}

void BurgerTime::CreatePlatform(Mage::GameObject* parent, const glm::vec2& position, bool isLight) const
{
	const std::string name = isLight ? "PlatformLight" : "PlatformDark";
	const auto object = parent->CreateChildObject(name);
	object->GetTransform()->SetPosition(position);

	const std::string texturePath = isLight ? "Level/Platform_Light.png" : "Level/Platform_Dark.png";
	object->CreateComponent<Mage::SpriteComponent>(Mage::ResourceManager::GetInstance().LoadTexture(texturePath, 16));
}

void BurgerTime::CreateLadder(Mage::GameObject* parent, const glm::vec2& position) const
{
	const auto object = parent->CreateChildObject("Ladder");
	object->GetTransform()->SetPosition(position);

	object->CreateComponent<Mage::SpriteComponent>(Mage::ResourceManager::GetInstance().LoadTexture("Level/Ladder.png", 16));
}