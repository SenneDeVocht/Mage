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

#include "FpsCounterComponent.h"
#include "HealthDisplayComponent.h"
#include "PointsDisplayComponent.h"
#include "PeterPepper.h"

// Other
#include "Mage/Input/Command.h"
#include "Mage/ResourceManagement/ResourceManager.h"

void BurgerTime::LoadGame() const
{
	const auto scene = Mage::SceneManager::GetInstance().CreateScene("DemoScene");

	// Background
	auto go = scene->CreateObject("Background");
	go->CreateComponent<Mage::SpriteComponent>("background.jpg");

	// Logo
	go = scene->CreateObject("Logo");
	go->GetTransform()->SetPosition(204, 75, 0);
	go->CreateComponent<Mage::SpriteComponent>("logo.png");

	// Text
	go = scene->CreateObject("SubTitle");
	go->GetTransform()->SetPosition(128, 150, 0);

	auto font = Mage::ResourceManager::GetInstance().LoadFont("Cyber16.ttf", 50);
	go->CreateComponent<Mage::TextComponent>("Made by Senne De Vocht", font, SDL_Color{ 240, 240, 240 }, false);
	go->CreateComponent<Mage::SpriteComponent>();

	// FPS Counter
	go = scene->CreateObject("FpsCounter");
	go->GetTransform()->SetPosition(5, 0, 0);

	font = Mage::ResourceManager::GetInstance().LoadFont("Cyber16.ttf", 35);
	go->CreateComponent<Mage::TextComponent>("FPS: 0", font, SDL_Color{ 210, 96, 63 }, false);
	go->CreateComponent<FpsCounterComponent>();
	go->CreateComponent<Mage::SpriteComponent>();

	// Peter Pepper 1
	auto peterPepperParent = scene->CreateObject("PeterPepper_01");

	// Peter Pepper
	go = peterPepperParent->CreateChildObject("PeterPepper");
	const auto peterPepper = go->CreateComponent<PeterPepper>(0);

	// Lives display
	go = peterPepperParent->CreateChildObject("LivesDisplay");
	go->GetTransform()->SetPosition(5, 450, 0);
	go->CreateComponent<Mage::TextComponent>("LIVES: 3", font, SDL_Color{ 136, 186, 116 }, false);
	go->CreateComponent<Mage::SpriteComponent>();
	auto livesDispay = go->CreateComponent<HealthDisplayComponent>();

	peterPepper->AddLivesObserver(livesDispay);

	// Points display
	go = peterPepperParent->CreateChildObject("PointsDisplay");
	go->GetTransform()->SetPosition(5, 430, 0);
	go->CreateComponent<Mage::TextComponent>("POINTS: 0", font, SDL_Color{ 136, 186, 116 }, false);
	go->CreateComponent<Mage::SpriteComponent>();
	auto pointsDispay = go->CreateComponent<PointsDisplayComponent>();

	peterPepper->AddPointsObserver(pointsDispay);

	// Peter Pepper 2
	peterPepperParent = scene->CreateObject("PeterPepper_02");

	// Peter Pepper
	go = peterPepperParent->CreateChildObject("PeterPepper");
	const auto peterPepper2 = go->CreateComponent<PeterPepper>(1);

	// Lives display
	go = peterPepperParent->CreateChildObject("LivesDisplay");
	go->GetTransform()->SetPosition(5, 400, 0);
	go->CreateComponent<Mage::TextComponent>("LIVES: 3", font, SDL_Color{ 136, 186, 116 }, false);
	go->CreateComponent<Mage::SpriteComponent>();
	livesDispay = go->CreateComponent<HealthDisplayComponent>();

	peterPepper2->AddLivesObserver(livesDispay);

	// Points display
	go = peterPepperParent->CreateChildObject("PointsDisplay");
	go->GetTransform()->SetPosition(5, 380, 0);
	go->CreateComponent<Mage::TextComponent>("POINTS: 0", font, SDL_Color{ 136, 186, 116 }, false);
	go->CreateComponent<Mage::SpriteComponent>();
	pointsDispay = go->CreateComponent<PointsDisplayComponent>();

	peterPepper2->AddPointsObserver(pointsDispay);

	std::cout << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "How to play:" << std::endl;
	std::cout << "X - Lose one life" << std::endl;
	std::cout << "Y - Gain one point" << std::endl;
	std::cout << std::endl;
	std::cout << "Only two players on controller supported" << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << std::endl;


	// Animation test
	go = scene->CreateObject("animation");
	go->CreateComponent<Mage::AnimatedSpriteComponent>("PeterPepper/WalkFront.png", 3, .2f);
	go->GetTransform()->SetPosition(200, 200);
}