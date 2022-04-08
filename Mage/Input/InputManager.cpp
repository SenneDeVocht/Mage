#include "Mage/MagePCH.h"
#include "InputManager.h"

#include "Mage/Input/Command.h"

#include "backends/imgui_impl_sdl.h"
#include "Xinput.h"
#include <vector>
#include <array>

class Mage::InputManager::InputManagerImpl
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
        // XInput
        memcpy(m_PreviousStates.data(), m_CurrentStates.data(), sizeof(m_CurrentStates.data()));
        for (DWORD i = 0; i < 4; i++)
        {
            XInputGetState(i, &(m_CurrentStates[i]));
        }

        for (auto& action : m_InputActions)
        {
            if (CheckButton(action->ControllerIndex, action->Button, action->State))
            {
                action->Command->Execute();
            }
        }

        // SDL Events
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
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

    void RemoveInputAction(InputAction* action)
    {
        delete action;
        const auto newEnd = std::remove(m_InputActions.begin(), m_InputActions.end(), action);
        m_InputActions.erase(newEnd, m_InputActions.end());
    }

    bool CheckButton(int controllerIndex, ControllerButton button, InputState state) const
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
    bool IsDown(int controllerIndex, WORD button) const
    {
        WORD changedButtons = m_PreviousStates[controllerIndex].Gamepad.wButtons ^ m_CurrentStates[controllerIndex].Gamepad.wButtons;
        WORD downButtons = changedButtons & m_CurrentStates[controllerIndex].Gamepad.wButtons;

        return button & downButtons;
    }
    bool IsHeld(int controllerIndex, WORD button) const
    {
        return button & m_CurrentStates[controllerIndex].Gamepad.wButtons;
    }
    bool IsUp(int controllerIndex, WORD button) const
    {
        WORD changedButtons = m_PreviousStates[controllerIndex].Gamepad.wButtons ^ m_CurrentStates[controllerIndex].Gamepad.wButtons;
        WORD upButtons = changedButtons & (~m_CurrentStates[controllerIndex].Gamepad.wButtons);

        return button & upButtons;
    }

    std::vector<InputAction*> m_InputActions{};

    std::array<XINPUT_STATE, 4> m_CurrentStates;
    std::array<XINPUT_STATE, 4> m_PreviousStates;
};


Mage::InputManager::InputManager()
    : m_pImpl{ std::make_unique<InputManagerImpl>() }
{
}

// Needed for unique pointer of incomplete type
Mage::InputManager::~InputManager()
{
}

bool Mage::InputManager::ProcessInput() const
{
    return m_pImpl->ProcessInput();
}

void Mage::InputManager::AddInputAction(InputAction* action) const
{
    m_pImpl->AddInputAction(action);
}

void Mage::InputManager::RemoveInputAction(InputAction* action) const
{
    m_pImpl->RemoveInputAction(action);
}

bool Mage::InputManager::CheckButton(int controllerIndex, ControllerButton button, InputState state) const
{
    return m_pImpl->CheckButton(controllerIndex, button, state);
}