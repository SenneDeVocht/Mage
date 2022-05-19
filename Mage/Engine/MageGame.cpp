#include "Mage/MagePCH.h"
#include "MageGame.h"

#include <thread>
#include <chrono>

// Services
#include "Mage/Engine/ServiceLocator.h"
#include "Mage/Engine/Renderer.h"
#include "Mage/Input/InputManager.h"
#include "Mage/Engine/SoundManager.h"

// Singletons
#include "Mage/Scenegraph/SceneManager.h"
#include "Mage/ResourceManagement/ResourceManager.h"
#include "Mage/Engine/Timer.h"

// ImGui
#include "Mage/ImGui/ImGuiHelper.h"
#include "imgui.h"
#include "backends/imgui_impl_opengl2.h"
#include "backends/imgui_impl_sdl.h"

// Scenegraph
#include "Mage/Engine/GameSettings.h"
#include "Mage/Scenegraph/GameObject.h"

Mage::MageGame::~MageGame()
{
	SDL_DestroyWindow(m_pWindow);
}

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

void Mage::MageGame::Initialize()
{
	// SDL
	PrintSDLVersion();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());

	m_pWindow = SDL_CreateWindow(
		GameSettings::GetInstance().GetWindowTitle().c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		(int)GameSettings::GetInstance().GetWindowSize().x,
		(int)GameSettings::GetInstance().GetWindowSize().y,
		SDL_WINDOW_OPENGL
	);

	if (m_pWindow == nullptr)
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());

	// Register services
	ServiceLocator::RegisterRenderer(new GLRenderer(m_pWindow));
	ServiceLocator::RegisterInputManager(new XInputInputManager());

#ifdef _DEBUG
	ServiceLocator::RegisterSoundManager(new DebugSDLSoundManager());
#else
	ServiceLocator::RegisterSoundManager(new SDLSoundManager());
#endif

	// imgui
	ImGuiHelper::InitImGui(m_pWindow);

	// Initialize random number generator
	srand((unsigned int)time(nullptr));
}


void Mage::MageGame::Cleanup()
{
	// Stop ImGui
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	// Stop services
	ServiceLocator::RegisterRenderer(nullptr);
	ServiceLocator::RegisterInputManager(nullptr);
	ServiceLocator::RegisterSoundManager(nullptr);

	// Stop SDL
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;
	SDL_Quit();
}

void Mage::MageGame::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();
	
	const auto& renderer = ServiceLocator::GetRenderer();
	const auto& sceneManager = SceneManager::GetInstance();
	const auto& input = ServiceLocator::GetInputManager();
	const auto& timer = Timer::GetInstance();

	bool quit = false;
	float lag = 0.0f;

	// Game Loop
	while (!quit)
	{
		// Add, Remove, Enable, Disable, ...
		sceneManager.ChangeSceneGraph();

		// Time calculations
		timer.CalculateTime();
		lag += timer.GetDeltaTime();

		// Input
		quit = !input->ProcessInput();

		// Update
		sceneManager.Update();

		// Fixed Update
		while (lag >= timer.GetFixedTimeStep())
		{
			sceneManager.FixedUpdate();
			lag -= timer.GetFixedTimeStep();
		}

		
		// Render
		renderer->Render();
	}

	Cleanup();
}