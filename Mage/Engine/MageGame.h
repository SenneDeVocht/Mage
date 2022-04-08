#pragma once

struct SDL_Window;

namespace Mage
{
	class MageGame
	{
	public:
		MageGame(){};
		virtual ~MageGame();

		MageGame(const MageGame& other) = delete;
		MageGame(MageGame&& other) = delete;
		MageGame& operator=(const MageGame& other) = delete;
		MageGame& operator=(MageGame&& other) = delete;


		void Initialize(int width, int height, const std::string& title);
		virtual void LoadGame() const = 0;
		void Cleanup();
		void Run(int width, int height, const std::string& title);

	private:
		SDL_Window* m_Window{};
	};
}

