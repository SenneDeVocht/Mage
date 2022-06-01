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
#include "Mage/Components/TextComponent.h"

#include "BurgerTime/MainMenu.h"
#include "BurgerTime/PlayerAndEnemies/PlayerMovement.h"
#include "BurgerTime/PlayerAndEnemies/PeterPepper.h"
#include "BurgerTime/Level.h"
#include "BurgerTime/PepperUI.h"
#include "BurgerTime/LivesUI.h"
#include "BurgerTime/PlayerAndEnemies/EnemyManager.h"

// Other
#include "Mage/Engine/ServiceLocator.h"
#include "Mage/Engine/Renderer.h"
#include "Mage/ResourceManagement/ResourceManager.h"

void BurgerTime::LoadGame() const
{
	const auto& resourceManager = Mage::ResourceManager::GetInstance();

	// MAIN MENU
	//----------
	Mage::SceneManager::GetInstance().RegisterScene({ "Level_01", [&](Mage::Scene* pScene)
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

			// PLAY TEXT
		    //----------
            #pragma region PlayText

			const auto playTextObject = pScene->CreateChildObject("PlayText");
		    const auto playText = playTextObject->CreateComponent<Mage::TextComponent>(
				"Press SPACE to Play",
				Mage::ResourceManager::GetInstance().LoadFont("Fonts/PressStart2P.ttf", 8),
				SDL_Color{ 255, 255, 255, 255 },
                16.f,
				glm::vec2{ 0.5f, 0.5f });
		    playTextObject->GetTransform()->SetWorldPosition({ 0, -3 });

            #pragma endregion

		    // MAIN MENU
		    //----------
            #pragma region MainMenu

		    const auto mainMenuObject = pScene->CreateChildObject("MainMenu");
		    mainMenuObject->CreateComponent<MainMenu>(playText);

            #pragma endregion
	    }
	});

	// LEVEL 01
	//---------
	Mage::SceneManager::GetInstance().RegisterScene({ "Level_01", [&](Mage::Scene* pScene)
	    {
	        // Camera
		    //-------
		    #pragma region Camera

		    const auto cameraObject = pScene->CreateChildObject("Camera");
		    cameraObject->GetTransform()->SetWorldPosition({ 0.0f, -0.5f });
		    const auto camera = cameraObject->CreateComponent<Mage::CameraComponent>(glm::vec2{ 15.f, 15.f });
		    Mage::ServiceLocator::GetRenderer()->SetCamera(camera);

		    #pragma endregion

		    // LEVEL
		    //------
		    #pragma region Level

		    const auto levelObject = pScene->CreateChildObject("Level");
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

		    // PETER PEPPER AND ENEMIES
		    //-------------------------
		    #pragma region PeterPepper

		    const auto peterPepperObject = pScene->CreateChildObject("PeterPepper");
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

            
		    const auto enemyManagerObject = pScene->CreateChildObject("EnemyManager");
		    const auto enemyManager = enemyManagerObject->CreateComponent<EnemyManager>(
			    std::vector<glm::vec2>{ { -6, 4.6875f }, { 6, 4.6875f }, { -6, -4.3125f }, { 6, -4.3125f } },
			    level, peterPepperObject->GetTransform());


		    const auto peterPepper = peterPepperObject->CreateComponent<PeterPepper>(level, enemyManager,
			    std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("PeterPepper/Victory.png", 16), 2, 0.2f),
			    std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("PeterPepper/Death.png", 16), 6, 0.1f, false));

		    #pragma endregion

		    // PEPPER UI
		    //----------
		    #pragma region PepperUI

		    const auto pepperUIObject = pScene->CreateChildObject("PepperUI");
		    pepperUIObject->GetTransform()->SetWorldPosition(glm::vec2{ 6.5f, 6.875f });

		    const auto pepperUIImage = pepperUIObject->CreateChildObject("Image");
		    pepperUIImage->CreateComponent<Mage::SpriteComponent>(resourceManager.LoadTexture("UI/PepperTitle.png", 16, glm::vec2{ 1, 1 }));

		    const auto PepperUITextObject = pepperUIObject->CreateChildObject("Text");
		    PepperUITextObject->GetTransform()->SetLocalPosition({ 0.f, -0.4375f });
		    PepperUITextObject->CreateComponent<Mage::TextComponent>("X", resourceManager.LoadFont("Fonts/PressStart2P.ttf", 8), SDL_Color{ 255, 255, 255, 255 }, 16.f, glm::vec2{ 1.f, 1.f }, Mage::TextComponent::TextAlignment::Right);
		    PepperUITextObject->CreateComponent<PepperUI>(peterPepper);

		    #pragma endregion

		    // LIVES UI
		    //----------
		    #pragma region PepperUI

		    const auto livesUIObject = pScene->CreateChildObject("LivesUI");
		    livesUIObject->GetTransform()->SetWorldPosition(glm::vec2{ -6.5f, -7.875f });
		    livesUIObject->CreateComponent<LivesUI>(peterPepper);

		    #pragma endregion
	    }
	});

	Mage::SceneManager::GetInstance().LoadScene(0);
}
