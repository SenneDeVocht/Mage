#include "MagePCH.h"
#include "InputManager.h"
#include "backends/imgui_impl_sdl.h"
#include "InputManager.h"
#include <vector>

#include "Command.h"
#include "Xinput.h"

class InputManager::InputManagerImpl
{
public:
    InputManagerImpl() = default;
    ~InputManagerImpl()
    {
        for (auto& action : m_InputActions)
        {
            delete action;
        }
        m_InputActions.clear();
    }

    InputManagerImpl(const InputManagerImpl& other) = delete;
    InputManagerImpl(InputManagerImpl&& other) noexcept = delete;
    InputManagerImpl& operator=(const InputManagerImpl& other) = delete;
    InputManagerImpl& operator=(InputManagerImpl&& other) noexcept = delete;

    bool ProcessInput()
    {
        // Get Input
        m_PreviousState = m_CurrentState;
        XInputGetState(0, &m_CurrentState);

        // Execute User-Defined Actions
        for (auto& action : m_InputActions)
        {
            if (action->State == InputState::Down && IsDown(ToXinputButton(action->Button)) ||
                action->State == InputState::Hold && IsHeld(ToXinputButton(action->Button)) ||
                action->State == InputState::Up && IsUp(ToXinputButton(action->Button)))
            {
                action->Command->Execute();
            }
        }

        // SDL Events
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            // Quit application
            if (e.type == SDL_QUIT)
                return false;

            // ImGui input
            ImGui_ImplSDL2_ProcessEvent(&e);
        }

        return true;
    }

    void AddInputAction(InputAction* action)
    {
        m_InputActions.push_back(action);
    }

private:
    static WORD ToXinputButton(ControllerButton button)
    {
        switch (button)
        {
        case ControllerButton::ButtonA:
            return XINPUT_GAMEPAD_A;

        case ControllerButton::ButtonB:
            return XINPUT_GAMEPAD_B;

        case ControllerButton::ButtonX:
            return XINPUT_GAMEPAD_X;

        case ControllerButton::ButtonY:
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

        case ControllerButton::RightThumb:
            return XINPUT_GAMEPAD_RIGHT_THUMB;

        case ControllerButton::LeftThumb:
            return XINPUT_GAMEPAD_LEFT_THUMB;

        case ControllerButton::Start:
            return XINPUT_GAMEPAD_START;

        case ControllerButton::Back:
            return XINPUT_GAMEPAD_BACK;

        default:
            return 0;
        }
    }
    bool IsDown(WORD button) const
    {
        WORD changedButtons = m_PreviousState.Gamepad.wButtons ^ m_CurrentState.Gamepad.wButtons;
        WORD downButtons = changedButtons & m_CurrentState.Gamepad.wButtons;

        return button & downButtons;
    }
    bool IsHeld(WORD button) const
    {
        return button & m_CurrentState.Gamepad.wButtons;
    }
    bool IsUp(WORD button) const
    {
        WORD changedButtons = m_PreviousState.Gamepad.wButtons ^ m_CurrentState.Gamepad.wButtons;
        WORD upButtons = changedButtons & (~m_CurrentState.Gamepad.wButtons);

        return button & upButtons;
    }

    std::vector<InputAction*> m_InputActions{};

    XINPUT_STATE m_CurrentState{};
    XINPUT_STATE m_PreviousState{};
};


InputManager::InputManager()
    : m_pImpl{ std::make_unique<InputManagerImpl>() }
{
}

// Needed for unique pointer of incomplete type
InputManager::~InputManager()
{
}

bool InputManager::ProcessInput() const
{
    return m_pImpl->ProcessInput();
}

void InputManager::AddInputAction(InputAction* action) const
{
    m_pImpl->AddInputAction(action);
}