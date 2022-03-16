#include "MagePCH.h"
#include "Mage.h"

#include <thread>
#include <chrono>

// Singletons
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Timer.h"

// ImGui
#include "imgui.h"
#include "backends/imgui_impl_opengl2.h"
#include "backends/imgui_impl_sdl.h"

// Steam
#include <steam_api.h>

// Scenegraph
#include "Scene.h"
#include "GameObject.h"

// Components
#include "AchievementsManager.h"
#include "Transform.h"
#include "RendererComponent.h"
#include "TextComponent.h"
#include "FpsCounterComponent.h"
#include "TrashTheCache.h"
#include "PeterPepper.h"
#include "HealthDisplayComponent.h"
#include "PointsDisplayComponent.h"

using namespace std;

void PrintSDLVersion()
{
	SDL_version compiled{};
	SDL_version linked{};

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	printf("We compiled against SDL version %d.%d.%d ...\n",
		compiled.major, compiled.minor, compiled.patch);
	printf("We are linking against SDL version %d.%d.%d.\n",
		linked.major, linked.minor, linked.patch);
}

void Mage::Initialize()
{
	// SDL
	PrintSDLVersion();

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());

	m_Window = SDL_CreateWindow(
		"Mage",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);

	if (m_Window == nullptr)
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());

	Renderer::GetInstance().Init(m_Window);

	// imgui
    IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(m_Window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

/**
 * Code constructing the scene world starts here
 */
void Mage::LoadGame() const
{
	const auto scene = SceneManager::GetInstance().CreateScene("DemoScene");

	// Background
	auto go = scene->CreateObject("Background");
	go->CreateComponent<RendererComponent>("background.jpg");

	// Logo
	go = scene->CreateObject("Logo");
	go->GetTransform()->SetPosition(204, 75, 0);
	go->CreateComponent<RendererComponent>("logo.png");

	// Text
	go = scene->CreateObject("SubTitle");
	go->GetTransform()->SetPosition(128, 150, 0);

	auto font = ResourceManager::GetInstance().LoadFont("Cyber16.ttf", 50);
	go->CreateComponent<TextComponent>("Made by Senne De Vocht", font, SDL_Color{ 240, 240, 240 }, false);
    go->CreateComponent<RendererComponent>();

	// FPS Counter
	go = scene->CreateObject("FpsCounter");
	go->GetTransform()->SetPosition(5, 0, 0);

	font = ResourceManager::GetInstance().LoadFont("Cyber16.ttf", 35);
	go->CreateComponent<TextComponent>("FPS: 0", font, SDL_Color{ 210, 96, 63 }, false);
	go->CreateComponent<FpsCounterComponent>();
	go->CreateComponent<RendererComponent>();

	// Peter Pepper 1
	auto peterPepperParent = scene->CreateObject("PeterPepper_01");

	// Peter Pepper
	go = peterPepperParent->CreateChildObject("PeterPepper");
	const auto peterPepper = go->CreateComponent<PeterPepper>(0);

	// Lives display
	go = peterPepperParent->CreateChildObject("LivesDisplay");
	go->GetTransform()->SetPosition(5, 450, 0);
	go->CreateComponent<TextComponent>("LIVES: 3", font, SDL_Color{ 136, 186, 116 }, false);
	go->CreateComponent<RendererComponent>();
	auto livesDispay = go->CreateComponent<HealthDisplayComponent>();

	peterPepper->AddLivesObserver(livesDispay);

	// Points display
	go = peterPepperParent->CreateChildObject("PointsDisplay");
	go->GetTransform()->SetPosition(5, 430, 0);
	go->CreateComponent<TextComponent>("POINTS: 0", font, SDL_Color{ 136, 186, 116 }, false);
	go->CreateComponent<RendererComponent>();
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
	go->CreateComponent<TextComponent>("LIVES: 3", font, SDL_Color{ 136, 186, 116 }, false);
	go->CreateComponent<RendererComponent>();
	livesDispay = go->CreateComponent<HealthDisplayComponent>();

	peterPepper2->AddLivesObserver(livesDispay);

	// Points display
	go = peterPepperParent->CreateChildObject("PointsDisplay");
	go->GetTransform()->SetPosition(5, 380, 0);
	go->CreateComponent<TextComponent>("POINTS: 0", font, SDL_Color{ 136, 186, 116 }, false);
	go->CreateComponent<RendererComponent>();
	pointsDispay = go->CreateComponent<PointsDisplayComponent>();

	peterPepper2->AddPointsObserver(pointsDispay);

	// Achievement manager
	go = scene->CreateObject("AchievementManager");
	const auto achievementManager = go->CreateComponent<AchievementsManager>();

	peterPepper->AddPointsObserver(achievementManager);
	peterPepper2->AddPointsObserver(achievementManager);

	std::cout << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "How to play:" << std::endl;
	std::cout << "X - Lose one life" << std::endl;
	std::cout << "Y - Gain one point" << std::endl;
	std::cout << std::endl;
	std::cout << "Only two players on controller supported" << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << std::endl;
}

void Mage::Cleanup()
{
	// Stop ImGui
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	// Stop SDL
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void Mage::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	{
		const auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		const auto& input = InputManager::GetInstance();
		const auto& timer = Timer::GetInstance();

		bool quit = false;
		auto lastTime = std::chrono::high_resolution_clock::now();
		float lag = 0.0f;

		// Game Loop
		while (!quit)
		{
			// Time calculations
			timer.CalculateTime();
			lag += timer.GetDeltaTime();

			// Input
			quit = !input.ProcessInput();

			// Steam
			SteamAPI_RunCallbacks();

			// ImGui
			// Prepare ImGui
			ImGui_ImplOpenGL2_NewFrame();
			ImGui_ImplSDL2_NewFrame(m_Window);
			ImGui::NewFrame();

			// Update
			sceneManager.DrawImGui();

			// Render ImGui
			ImGui::Render();

			// Update
		    sceneManager.Update();

			// Fixed Update
			while (lag >= timer.GetFixedTimeStep())
			{
				sceneManager.FixedUpdate();
				lag -= timer.GetFixedTimeStep();
			}

			// Destroy Objects
			sceneManager.DestroyMarkedObjects();

			// Render
			renderer.Render();
		}

		Cleanup();
	}
}
