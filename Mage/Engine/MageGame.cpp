#include "Mage/MagePCH.h"
#include "MageGame.h"

#include <thread>
#include <chrono>

// Singletons
#include "Mage/Input/InputManager.h"
#include "Mage/Scenegraph/SceneManager.h"
#include "Mage/ResourceManagement/ResourceManager.h"
#include "Mage/Engine/Renderer.h"
#include "Mage/Engine/Timer.h"

// ImGui
#include "imgui.h"
#include "backends/imgui_impl_opengl2.h"
#include "backends/imgui_impl_sdl.h"

// Scenegraph
#include "../Scenegraph/GameObject.h"

Mage::MageGame::~MageGame()
{
	SDL_DestroyWindow(m_Window);
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

void Mage::MageGame::Initialize(int width, int height, const std::string& title)
{
	// SDL
	PrintSDLVersion();

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());

	m_Window = SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
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


void Mage::MageGame::Cleanup()
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

void Mage::MageGame::Run(int width, int height, const std::string& title)
{
	Initialize(width, height, title);

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