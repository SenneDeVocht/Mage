#pragma once
#include "../Singleton.h"

namespace Mage
{
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
		int ControllerIndex;
		ControllerButton Button;

		int KeyboardKey;

		InputState State;
		std::unique_ptr<Command> Command;
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager() override;

		bool ProcessInput() const;
		void AddInputAction(InputAction* action) const;
		void RemoveInputAction(InputAction* action) const;

		bool CheckGamepadButton(int controllerIndex, ControllerButton button, InputState state) const;
		bool CheckKeyboardKey(int keyboardKey, InputState state) const;

	private:
		class InputManagerImpl;
		std::unique_ptr<InputManagerImpl> m_pImpl;
	};
}
