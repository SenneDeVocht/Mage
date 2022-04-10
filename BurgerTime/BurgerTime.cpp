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
#include "PeterPepper.h"

// Other
#include "Mage/Engine/Renderer.h"
#include "Mage/Input/Command.h"
#include "Mage/ResourceManagement/ResourceManager.h"

void BurgerTime::LoadGame() const
{
	const auto scene = Mage::SceneManager::GetInstance().CreateScene("DemoScene");

	// Camera
	{
		const auto cameraObject = scene->CreateObject("Camera");
		const auto camera = cameraObject->CreateComponent<Mage::CameraComponent>(16.f, 16.f);
		Mage::Renderer::GetInstance().SetCamera(camera);
	}

	// Level
	{
		
	}

	// Peter Pepper
	{
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
	}
}