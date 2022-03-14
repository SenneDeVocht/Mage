#pragma once
#include "Singleton.h"

class Command;

enum class InputState
{
	Down,
	Hold,
	Up
};

enum class ControllerButton
{
	ButtonA,
	ButtonB,
	ButtonX,
	ButtonY,
	DPadUp,
	DPadDown,
	DPadLeft,
	DPadRight,
	LeftShoulder,
	RightShoulder,
	RightThumb,
	LeftThumb,
	Start,
	Back
};

struct InputAction
{
	ControllerButton Button;
	InputState State;
	std::unique_ptr<Command> Command;
};

class InputManager final : public Singleton<InputManager>
{
public:
	InputManager();
	~InputManager();
	bool ProcessInput() const;
	void AddInputAction(InputAction* action) const;

private:
	class InputManagerImpl;
	std::unique_ptr<InputManagerImpl> m_pImpl;
};