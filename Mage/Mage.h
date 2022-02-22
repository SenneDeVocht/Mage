#pragma once

struct SDL_Window;

class Mage
{
public:
	void Initialize();
	void LoadGame() const;
	void Cleanup();
	void Run();

private:
	SDL_Window* m_Window{};

	float m_FixedTimeStep = 0.02f;
};