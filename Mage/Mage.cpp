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

// Scenegraph
#include "Scene.h"
#include "GameObject.h"

// Components
#include "RendererComponent.h"
#include "TextComponent.h"
#include "FpsCounterComponent.h"
#include "TrashTheCache.h"

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
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	// Background
	auto go = std::make_unique<GameObject>("Background");
	auto rc = std::make_unique<RendererComponent>();
	rc->SetTexture("background.jpg");
	go->AddComponent(std::move(rc));
	scene.Add(std::move(go));

	// Logo
	go = std::make_unique<GameObject>("Logo");
	rc = std::make_unique<RendererComponent>();
	rc->SetTexture("logo.png");
	go->AddComponent(std::move(rc));
	go->GetTransform()->SetPosition(204, 75, 0);
	scene.Add(std::move(go));

	// Text
	auto font = ResourceManager::GetInstance().LoadFont("Cyber16.ttf", 50);
	go = std::make_unique<GameObject>("SubTitle");
	go->GetTransform()->SetPosition(128, 150, 0);
	rc = std::make_unique<RendererComponent>();
	go->AddComponent(std::move(rc));
	auto tc = std::make_unique<TextComponent>("Made by Senne De Vocht", font, SDL_Color{ 240, 240, 240 }, false);
	go->AddComponent(std::move(tc));
	scene.Add(std::move(go));

	// FPS Counter
	go = std::make_unique<GameObject>("FpsCounter");
	go->GetTransform()->SetPosition(5, 0, 0);
	rc = std::make_unique<RendererComponent>();
	go->AddComponent(std::move(rc));
	font = ResourceManager::GetInstance().LoadFont("Cyber16.ttf", 35);
	tc = std::make_unique<TextComponent>("FPS: 0", font, SDL_Color{ 210, 96, 63 }, false);
	go->AddComponent(std::move(tc));
	auto fc = std::make_unique<FpsCounterComponent>();
	go->AddComponent(std::move(fc));
	scene.Add(std::move(go));


	// Cache trasher
	//go = std::make_shared<GameObject>("CacheTrasher");
	//auto ttc = std::make_shared<TrashTheCache>();
	//go->AddComponent(ttc);
	//scene.Add(go);
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
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		auto& timer = Timer::GetInstance();
		timer.SetFixedTimeStep(m_FixedTimeStep);

		bool quit = false;
		auto lastTime = std::chrono::high_resolution_clock::now();
		float lag = 0.0f;

		// Loop
		while (!quit)
		{
			// Time calculations
			const auto currentTime = std::chrono::high_resolution_clock::now();
			float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
			timer.SetDeltaTime(deltaTime);
			lastTime = currentTime;
			lag += deltaTime;

			// Input
			quit = !input.ProcessInput();

			// Update
		    {
			    // Prepare ImGui
				ImGui_ImplOpenGL2_NewFrame();
				ImGui_ImplSDL2_NewFrame(m_Window);
				ImGui::NewFrame();

				// Update
				sceneManager.Update();

				// Render ImGui
				ImGui::Render();
			}

			// Fixed Update
			while (lag >= m_FixedTimeStep)
			{
				sceneManager.FixedUpdate();
				lag -= m_FixedTimeStep;
			}

			// Destroy Objects
			sceneManager.DestroyMarkedObjects();

			// Render
			renderer.Render();
		}

		Cleanup();
	}
}
