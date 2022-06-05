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
#include "Mage/Components/TilemapComponent.h"
#include "Mage/Components/TextComponent.h"
#include "Mage/Components/SoundPlayerComponent.h"

#include "BurgerTime/Level.h"
#include "BurgerTime/Managers/GameManager.h"
#include "BurgerTime/Managers/ScoreManager.h"
#include "BurgerTime/UI/MainMenu.h"
#include "BurgerTime/UI/LivesUI.h"
#include "BurgerTime/PlayerAndEnemies/EnemyManager.h"
#include "BurgerTime/UI/ScoreUI.h"
#include "BurgerTime/UI/GameOver.h"
#include "BurgerTime/UI/HighScoreUI.h"

// Other
#include "Mage/Engine/ServiceLocator.h"
#include "Mage/Engine/Renderer.h"
#include "Mage/ResourceManagement/ResourceManager.h"

void BurgerTime::LoadGame() const
{
	const auto& resourceManager = Mage::ResourceManager::GetInstance();

	// MAIN MENU
	//----------
	Mage::SceneManager::GetInstance().RegisterScene("MainMenu", [&](Mage::Scene* pScene)
	    {
		    // Camera
		    //-------
		    #pragma region Camera

		    const auto cameraObject = pScene->CreateChildObject("Camera");
		    const auto camera = cameraObject->CreateComponent<Mage::CameraComponent>(glm::vec2{ 15.f, 15.f });
		    Mage::ServiceLocator::GetRenderer()->SetCamera(camera);

		    #pragma endregion

            // TITLE
		    //------
            #pragma region Title

            const auto titleObject = pScene->CreateChildObject("Title");
            titleObject->CreateComponent<Mage::SpriteComponent>(Mage::ResourceManager::GetInstance().LoadTexture("UI/Title.png", 16.f));
			titleObject->GetTransform()->SetWorldPosition({ 0, 3 });

            #pragma endregion

			// OPTIONS TEXT
		    //-------------
			#pragma region OptionsText

			const auto optionsTextObject = pScene->CreateChildObject("OptionsText");
			const auto optionsText = optionsTextObject->CreateComponent<Mage::TextComponent>(
				"> 1 PLAYER\n\n  2 PLAYERS",
				Mage::ResourceManager::GetInstance().LoadFont("Fonts/PressStart2P.ttf", 8),
				SDL_Color{ 255, 255, 255, 255 },
				16.f,
				glm::vec2{ 0.5f, 0.5f },
				Mage::TextComponent::TextAlignment::Left);
			optionsTextObject->GetTransform()->SetWorldPosition({ 0, -2 });

            #pragma endregion

			// HIGH SCORE TEXT
			//-----------------
			#pragma region HighScoreText

			const auto highScoreTextObject = pScene->CreateChildObject("HighScoreText");
			highScoreTextObject->CreateComponent<Mage::TextComponent>(
				"High Score: 0",
				Mage::ResourceManager::GetInstance().LoadFont("Fonts/PressStart2P.ttf", 8),
				SDL_Color{ 255, 255, 255, 255 },
				16.f,
				glm::vec2{ 0.5f, 0.5f });
			highScoreTextObject->CreateComponent<HighScoreUI>();
			highScoreTextObject->GetTransform()->SetWorldPosition({ 0, -6 });

			#pragma endregion

		    // MAIN MENU
		    //----------
            #pragma region MainMenu

		    const auto mainMenuObject = pScene->CreateChildObject("MainMenu");
		    mainMenuObject->CreateComponent<MainMenu>(optionsText);

            #pragma endregion
	    });

	// LEVEL
	//------
	Mage::SceneManager::GetInstance().RegisterScene("Level01", [&](Mage::Scene* pScene)
	    {
	        // Camera
		    //-------
		    #pragma region Camera

		    const auto cameraObject = pScene->CreateChildObject("Camera");
		    cameraObject->GetTransform()->SetWorldPosition({ 0.0f, -0.5f });
		    const auto camera = cameraObject->CreateComponent<Mage::CameraComponent>(glm::vec2{ 15.f, 15.f });
		    Mage::ServiceLocator::GetRenderer()->SetCamera(camera);

		    #pragma endregion

			// GAME MANAGER
			//-------------
			#pragma region GameManager

			const auto gameManagerObject = pScene->CreateChildObject("GameManager");
			const auto gameManager = gameManagerObject->CreateComponent<GameManager>();

			#pragma endregion

		    // LEVEL
		    //------
		    #pragma region Level

		    const auto levelObject = pScene->CreateChildObject("Level");
		    levelObject->GetTransform()->SetWorldPosition({ 0.f, -0.5f });
			const auto startSound = levelObject->CreateComponent<Mage::SoundPlayerComponent>(Mage::ResourceManager::GetInstance().LoadSoundClip("Sounds/Start.mp3"));
			const auto music = levelObject->CreateComponent<Mage::SoundPlayerComponent>(Mage::ResourceManager::GetInstance().LoadSoundClip("Sounds/Music.mp3"), 1.f, true);
			const auto victorySound = levelObject->CreateComponent<Mage::SoundPlayerComponent>(Mage::ResourceManager::GetInstance().LoadSoundClip("Sounds/Complete.mp3"));
			const auto level = levelObject->CreateComponent<Level>(gameManager, startSound, music, victorySound);
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

			// ENEMIES
			//--------
			#pragma region Enemies

		    const auto enemyManagerObject = pScene->CreateChildObject("EnemyManager");
		    const auto enemyManager = enemyManagerObject->CreateComponent<EnemyManager>(gameManager, level);

		    #pragma endregion

			// SCORE MANAGER
			//--------------
			#pragma region ScoreManager

			const auto scoreManagerObject = pScene->CreateChildObject("ScoreManager");
			const auto scoreManager = scoreManagerObject->CreateComponent<ScoreManager>(gameManager, level, enemyManager);

			#pragma endregion

			// LIVES UI
			//----------
			#pragma region LivesUI

			const auto livesUIObject = pScene->CreateChildObject("LivesUI");
			livesUIObject->GetTransform()->SetWorldPosition(glm::vec2{ -6.5f, -7.875f });
			livesUIObject->CreateComponent<LivesUI>(gameManager);

			#pragma endregion

			// SCORE UI
			//---------
			#pragma region ScoreUI

			const auto scoreUIObject = pScene->CreateChildObject("ScoreUI");
			scoreUIObject->GetTransform()->SetWorldPosition(glm::vec2{ 0.f, 6.875f });
			scoreUIObject->CreateComponent<ScoreUI>(scoreManager);
			scoreUIObject->CreateComponent<Mage::TextComponent>("420", resourceManager.LoadFont("Fonts/PressStart2P.ttf", 8), SDL_Color{255, 255, 255, 255}, 16.f, glm::vec2{0.5f, 1.f}, Mage::TextComponent::TextAlignment::Left);

			#pragma endregion
	    });

	// GAME OVER
	//----------
	Mage::SceneManager::GetInstance().RegisterScene("GameOver", [&](Mage::Scene* pScene)
		{
			// Camera
			//-------
			#pragma region Camera

			const auto cameraObject = pScene->CreateChildObject("Camera");
			const auto camera = cameraObject->CreateComponent<Mage::CameraComponent>(glm::vec2{ 15.f, 15.f });
			Mage::ServiceLocator::GetRenderer()->SetCamera(camera);

			#pragma endregion

			// TEXT
			//-----
			#pragma region Text

			const auto textObject = pScene->CreateChildObject("GameOverText");
			textObject->CreateComponent<Mage::TextComponent>(
				"GAME OVER",
				Mage::ResourceManager::GetInstance().LoadFont("Fonts/PressStart2P.ttf", 8),
				SDL_Color{ 255, 255, 255, 255 },
				16.f,
				glm::vec2{ 0.5f, 0.5f });

			#pragma endregion

			// MANAGER
			//--------
			#pragma region MainMenu

			const auto gameOverObject = pScene->CreateChildObject("GameOverManager");
			gameOverObject->CreateComponent<GameOver>();

			#pragma endregion
		});

	Mage::SceneManager::GetInstance().LoadScene("MainMenu");
}
