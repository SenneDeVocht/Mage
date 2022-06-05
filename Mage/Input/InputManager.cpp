#include "Mage/MagePCH.h"
#include "InputManager.h"

#include "Mage/Input/Command.h"

#include "backends/imgui_impl_sdl.h"
#include "Xinput.h"
#include <vector>
#include <array>

class Mage::XInputInputManager::XInputInputManagerImpl
{
public:
    XInputInputManagerImpl() = default;
    ~XInputInputManagerImpl()
    {
        for (auto& action : m_InputActions)
        {
            delete action;
        }
        m_InputActions.clear();
    }

    XInputInputManagerImpl(const XInputInputManagerImpl& other) = delete;
    XInputInputManagerImpl(XInputInputManagerImpl&& other) noexcept = delete;
    XInputInputManagerImpl& operator=(const XInputInputManagerImpl& other) = delete;
    XInputInputManagerImpl& operator=(XInputInputManagerImpl&& other) noexcept = delete;

    bool ProcessInput()
    {
        // Controller
        m_PreviousControllerStates = m_CurrentControllerStates;
        for (DWORD i = 0; i < 4; i++)
        {
            XInputGetState(i, &(m_CurrentControllerStates[i]));
        }

        // Keyboard
        memcpy(&m_PreviousKeyboardState, &m_CurrentKeyboardState, sizeof(BYTE) * 256);
        GetKeyboardState(m_CurrentKeyboardState);

        for (auto& action : m_InputActions)
        {
	        if (action->ControllerIndex >= 0)
	        {
                if (CheckControllerButton(action->ControllerIndex, action->Button, action->State))
                {
                    action->Command->Execute();
                }
	        }
	        if (action->KeyboardKey >= 0)
	        {
                if (CheckKeyboardKey(action->KeyboardKey, action->State))
                {
                    action->Command->Execute();
                }
	        }
        }

        return true;
    }

    void AddInputAction(InputAction* action)
    {
        m_InputActions.push_back(action);
    }

    void RemoveInputAction(InputAction* action)
    {
        delete action;
        const auto newEnd = std::remove(m_InputActions.begin(), m_InputActions.end(), action);
        m_InputActions.erase(newEnd, m_InputActions.end());
    }

    bool IsControllerConnected(int controllerIndex) const
	{
		return m_CurrentControllerStates[controllerIndex].dwPacketNumber != 0;
	}

    bool CheckControllerButton(int controllerIndex, ControllerButton button, InputState state) const
    {
        switch (state)
        {
        case InputState::Down:
            return IsDown(controllerIndex, ToXinputButton(button));

        case InputState::Hold:
            return IsHeld(controllerIndex, ToXinputButton(button));

        case InputState::Up:
            return IsUp(controllerIndex, ToXinputButton(button));
        }

        return false;
    }

    float GetControllerAxis(int controllerIndex, ControllerAxis axis) const
    {
        switch (axis)
        {
            case ControllerAxis::LeftStickX:
				return m_CurrentControllerStates[controllerIndex].Gamepad.sThumbLX / 32767.0f;
            case ControllerAxis::LeftStickY:
				return m_CurrentControllerStates[controllerIndex].Gamepad.sThumbLY / 32767.0f;
            case ControllerAxis::RightStickX:
				return m_CurrentControllerStates[controllerIndex].Gamepad.sThumbRX / 32767.0f;
            case ControllerAxis::RightStickY:
				return m_CurrentControllerStates[controllerIndex].Gamepad.sThumbRY / 32767.0f;

            case ControllerAxis::LeftTrigger:
				return m_CurrentControllerStates[controllerIndex].Gamepad.bLeftTrigger / 255.0f;
            case ControllerAxis::RightTrigger:
				return m_CurrentControllerStates[controllerIndex].Gamepad.bRightTrigger / 255.0f;
        }

        return 0.0f;
    }

    bool CheckKeyboardKey(int keyboardKey, InputState state) const
    {
        switch (state)
        {
        case InputState::Down:
            return IsDown(keyboardKey);

        case InputState::Hold:
            return IsHeld(keyboardKey);

        case InputState::Up:
            return IsUp(keyboardKey);
        }

        return false;
    }

private:
    // Controller
    static WORD ToXinputButton(ControllerButton button)
    {
        switch (button)
        {
        case ControllerButton::A:
            return XINPUT_GAMEPAD_A;

        case ControllerButton::B:
            return XINPUT_GAMEPAD_B;

        case ControllerButton::X:
            return XINPUT_GAMEPAD_X;

        case ControllerButton::Y:
            return XINPUT_GAMEPAD_Y;

        case ControllerButton::DPadUp:
            return XINPUT_GAMEPAD_DPAD_UP;

        case ControllerButton::DPadDown:
            return XINPUT_GAMEPAD_DPAD_DOWN;

        case ControllerButton::DPadLeft:
            return XINPUT_GAMEPAD_DPAD_LEFT;

        case ControllerButton::DPadRight:
            return XINPUT_GAMEPAD_DPAD_RIGHT;

        case ControllerButton::LeftShoulder:
            return XINPUT_GAMEPAD_LEFT_SHOULDER;

        case ControllerButton::RightShoulder:
            return XINPUT_GAMEPAD_RIGHT_SHOULDER;

        case ControllerButton::RightStick:
            return XINPUT_GAMEPAD_RIGHT_THUMB;

        case ControllerButton::LeftStick:
            return XINPUT_GAMEPAD_LEFT_THUMB;

        case ControllerButton::Start:
            return XINPUT_GAMEPAD_START;

        case ControllerButton::Back:
            return XINPUT_GAMEPAD_BACK;

        default:
            return 0;
        }
    }
    bool IsDown(int controllerIndex, WORD button) const
    {
        WORD changedButtons = m_PreviousControllerStates[controllerIndex].Gamepad.wButtons ^ m_CurrentControllerStates[controllerIndex].Gamepad.wButtons;
        WORD downButtons = changedButtons & m_CurrentControllerStates[controllerIndex].Gamepad.wButtons;

        return button & downButtons;
    }
    bool IsHeld(int controllerIndex, WORD button) const
    {
        return button & m_CurrentControllerStates[controllerIndex].Gamepad.wButtons;
    }
    bool IsUp(int controllerIndex, WORD button) const
    {
        WORD changedButtons = m_PreviousControllerStates[controllerIndex].Gamepad.wButtons ^ m_CurrentControllerStates[controllerIndex].Gamepad.wButtons;
        WORD upButtons = changedButtons & (~m_CurrentControllerStates[controllerIndex].Gamepad.wButtons);

        return button & upButtons;
    }

    // Keyboard
    bool IsDown(int keyboardKey) const
    {
        return (m_CurrentKeyboardState[keyboardKey] & 0x80) && !(m_PreviousKeyboardState[keyboardKey] & 0x80);
    }
    bool IsHeld(int keyboardKey) const
    {
        return m_CurrentKeyboardState[keyboardKey] & 0x80;
    }
    bool IsUp(int keyboardKey) const
    {
        return !(m_CurrentKeyboardState[keyboardKey] & 0x80) && (m_PreviousKeyboardState[keyboardKey] & 0x80);
    }

    std::vector<InputAction*> m_InputActions{};

    std::array<XINPUT_STATE, 4> m_CurrentControllerStates;
    std::array<XINPUT_STATE, 4> m_PreviousControllerStates;

    BYTE m_CurrentKeyboardState[256];
    BYTE m_PreviousKeyboardState[256];
};


Mage::XInputInputManager::XInputInputManager()
    : m_pImpl{ std::make_unique<XInputInputManagerImpl>() }
{
}

// Needed for unique pointer of incomplete type
Mage::XInputInputManager::~XInputInputManager()
{
}

bool Mage::XInputInputManager::ProcessInput() const
{
    return m_pImpl->ProcessInput();
}

void Mage::XInputInputManager::AddInputAction(InputAction* action) const
{
    m_pImpl->AddInputAction(action);
}

void Mage::XInputInputManager::RemoveInputAction(InputAction* action) const
{
    m_pImpl->RemoveInputAction(action);
}

bool Mage::XInputInputManager::IsControllerConnected(int controllerIndex) const
{
	return m_pImpl->IsControllerConnected(controllerIndex);
}

bool Mage::XInputInputManager::CheckControllerButton(int controllerIndex, ControllerButton button, InputState state) const
{
    return m_pImpl->CheckControllerButton(controllerIndex, button, state);
}

float Mage::XInputInputManager::GetControllerAxis(int controllerIndex, ControllerAxis axis) const
{
	return m_pImpl->GetControllerAxis(controllerIndex, axis);
}

bool Mage::XInputInputManager::CheckKeyboardKey(int keyboardKey, InputState state) const
{
    return m_pImpl->CheckKeyboardKey(keyboardKey, state);
}
