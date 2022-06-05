#include "BurgerTime/BurgerTimePCH.h"
#include "GameManager.h"

#include "BurgerTime/LivesUI.h"
#include "BurgerTime/PepperUI.h"
#include "BurgerTime/SavedScoreManager.h"
#include "BurgerTime/ScoreManager.h"
#include "BurgerTime/SettingsManager.h"
#include "BurgerTime/Level.h"
#include "BurgerTime/PlayerAndEnemies/PeterPepper.h"
#include "BurgerTime/PlayerAndEnemies/EnemyManager.h"
#include "BurgerTime/PlayerAndEnemies/PlayerMovement.h"

#include "Mage/Components/BoxColliderComponent.h"
#include "Mage/Components/Transform.h"
#include "Mage/Components/RigidBodyComponent.h"
#include "Mage/Components/AnimatedSpriteComponent.h"
#include "Mage/Components/SpriteComponent.h"
#include "Mage/Components/TextComponent.h"
#include "Mage/Engine/Timer.h"
#include "Mage/ResourceManagement/ResourceManager.h"
#include "Mage/Scenegraph/Scene.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Scenegraph/SceneManager.h"

void GameManager::Initialize()
{
	// Spawn the player(s)
	SpawnPlayer(0);

	if (SettingsManager::GetInstance().GetGameMode() == SettingsManager::GameMode::Multiplayer)
	{
		SpawnPlayer(1);
	}
}

void GameManager::Update()
{
	if (m_LevelCompleted)
	{
		m_VictoryTimer += Mage::Timer::GetInstance().GetDeltaTime();

		if (m_VictoryTimer >= m_VictoryLength)
		{
			m_LevelCompleted = false;
			m_VictoryTimer = 0.0f;
			GoToNextLevel();
		}
	}

	int numDeadPlayers = 0;
	for (const auto p : m_pPeterPeppers)
	{
		if (p->IsDeadAndReady())
			numDeadPlayers++;
	}

	if (numDeadPlayers >= m_pPeterPeppers.size())
	{
		m_LivesLeft--;

		if (m_LivesLeft <= 0)
		{
			GameOver();
			return;
		}

		for (const auto p : m_pPeterPeppers)
		{
			p->Reset();
		}

		m_pEnemyManager->Reset();
	}
}

void GameManager::OnLevelCompleted()
{
	m_LevelCompleted = true;

	for (const auto p : m_pPeterPeppers)
	{
		p->StartVictory();
	}
}

void GameManager::SpawnPlayer(int playerIndex)
{
	const auto& resourceManager = Mage::ResourceManager::GetInstance();

	// PETER PEPPER
	//-------------

	const auto peterPepperObject = GetGameObject()->GetScene()->CreateChildObject("PeterPepper");
	peterPepperObject->SetTag("PeterPepper");
	peterPepperObject->GetTransform()->SetLocalPosition({ 0, -4.3125f });

	peterPepperObject->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Dynamic, true, 0.f);
	peterPepperObject->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 0.5f, 0.5f }, glm::vec2{ 0.f, -0.25f }, 0.f, true);
	peterPepperObject->CreateComponent<Mage::AnimatedSpriteComponent>();
	peterPepperObject->CreateComponent<PlayerMovement>(playerIndex, m_pLevel,
		std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("PeterPepper/Idle.png", 16), 1, 0.f),
		std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("PeterPepper/WalkFront.png", 16), 4, 0.1f),
		std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("PeterPepper/WalkBack.png", 16), 4, 0.1f),
		std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("PeterPepper/WalkLeft.png", 16), 4, 0.1f),
		std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("PeterPepper/WalkRight.png", 16), 4, 0.1f));
	const auto peterPepper = peterPepperObject->CreateComponent<PeterPepper>(
		playerIndex, m_pLevel,
		std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("PeterPepper/Victory.png", 16), 2, 0.2f),
		std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("PeterPepper/Death.png", 16), 6, 0.1f, false));

	m_pPeterPeppers.push_back(peterPepper);
	m_pEnemyManager->AddPlayer(peterPepper);
	

	// PEPPER UI
	//----------
	
	if (playerIndex == 0)
	{
		const auto pepperUIObject = GetGameObject()->GetScene()->CreateChildObject("PepperUI");
		pepperUIObject->GetTransform()->SetWorldPosition(glm::vec2{ -6.5f, 6.875f });

		const auto pepperUIImage = pepperUIObject->CreateChildObject("Image");
		pepperUIImage->CreateComponent<Mage::SpriteComponent>(resourceManager.LoadTexture("UI/PepperTitle.png", 16, glm::vec2{ 0, 1 }));

		const auto PepperUITextObject = pepperUIObject->CreateChildObject("Text");
		PepperUITextObject->GetTransform()->SetLocalPosition({ 0.f, -0.4375f });
		PepperUITextObject->CreateComponent<Mage::TextComponent>("X", resourceManager.LoadFont("Fonts/PressStart2P.ttf", 8), SDL_Color{ 255, 255, 255, 255 }, 16.f, glm::vec2{ 0.f, 1.f }, Mage::TextComponent::TextAlignment::Right);
		PepperUITextObject->CreateComponent<PepperUI>(peterPepper);
	}
	else
	{
		const auto pepperUIObject = GetGameObject()->GetScene()->CreateChildObject("PepperUI");
		pepperUIObject->GetTransform()->SetWorldPosition(glm::vec2{ 6.5f, 6.875f });

		const auto pepperUIImage = pepperUIObject->CreateChildObject("Image");
		pepperUIImage->CreateComponent<Mage::SpriteComponent>(resourceManager.LoadTexture("UI/PepperTitle.png", 16, glm::vec2{ 1, 1 }));

		const auto PepperUITextObject = pepperUIObject->CreateChildObject("Text");
		PepperUITextObject->GetTransform()->SetLocalPosition({ 0.f, -0.4375f });
		PepperUITextObject->CreateComponent<Mage::TextComponent>("X", resourceManager.LoadFont("Fonts/PressStart2P.ttf", 8), SDL_Color{ 255, 255, 255, 255 }, 16.f, glm::vec2{ 1.f, 1.f }, Mage::TextComponent::TextAlignment::Right);
		PepperUITextObject->CreateComponent<PepperUI>(peterPepper);
	}
}


void GameManager::GoToNextLevel()
{
	++m_CurrentLevel;

	if (m_CurrentLevel > 3)
	{
		GameOver();
	}
	else
	{
		m_pLevel->LoadLevel(m_CurrentLevel);

		for (const auto p : m_pPeterPeppers)
		{
			p->Reset();
		}
		m_pEnemyManager->Reset();
	}
}

void GameManager::GameOver()
{
	const int score = m_pScoreManager->GetScore();
	if (score > SavedScoreManager::GetSavedScore())
	{
		SavedScoreManager::SetSavedScore(score);
	}

	Mage::SceneManager::GetInstance().LoadScene("GameOver");
}
