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

#include "FpsCounterComponent.h"
#include "PeterPepper.h"

// Other
#include "Mage/Engine/Renderer.h"
#include "Mage/Input/Command.h"
#include "Mage/ResourceManagement/ResourceManager.h"

void BurgerTime::LoadGame() const
{
	const auto scene = Mage::SceneManager::GetInstance().CreateScene("DemoScene");

	// Camera
	auto go = scene->CreateObject("Camera");
	auto cam = go->CreateComponent<Mage::CameraComponent>(10.f, 480.f / 64.f);
	Mage::Renderer::GetInstance().SetCamera(cam);

	// Background
	go = scene->CreateObject("Background");
	go->CreateComponent<Mage::SpriteComponent>(Mage::ResourceManager::GetInstance().LoadTexture("background.png", 16));

	// Logo
	go = scene->CreateObject("Logo");
	go->GetTransform()->SetPosition(0, 1 + 3/32.f);
	go->CreateComponent<Mage::SpriteComponent>(Mage::ResourceManager::GetInstance().LoadTexture("logo.png", 16));

	// Text
	go = scene->CreateObject("SubTitle");
	go->GetTransform()->SetPosition(0, -1);

	auto font = Mage::ResourceManager::GetInstance().LoadFont("Cyber16.ttf", 15);
	go->CreateComponent<Mage::TextComponent>("Made by Senne De Vocht", font, SDL_Color{ 240, 240, 240 }, 16.f, Mage::TextComponent::HorizontalAlignment::Middle);
	go->CreateComponent<Mage::SpriteComponent>();

	// FPS Counter
	go = scene->CreateObject("FpsCounter");
	go->GetTransform()->SetPosition(-4.875f, 3.75f);

	font = Mage::ResourceManager::GetInstance().LoadFont("Cyber16.ttf", 15);
	go->CreateComponent<Mage::TextComponent>("FPS: 0", font, SDL_Color{ 210, 96, 63 }, 32.f, Mage::TextComponent::HorizontalAlignment::Left, Mage::TextComponent::VerticalAlignment::Top);
	go->CreateComponent<FpsCounterComponent>();
	go->CreateComponent<Mage::SpriteComponent>();

	// Peter Pepper
	{
		auto peterPepperObject = scene->CreateObject("PeterPepper");

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