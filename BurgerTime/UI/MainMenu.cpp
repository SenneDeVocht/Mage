#include "BurgerTime/BurgerTimePCH.h"
#include "MainMenu.h"

#include "Mage/Components/TextComponent.h"
#include "Mage/Engine/ServiceLocator.h"
#include "Mage/Input/InputManager.h"
#include "Mage/Scenegraph/SceneManager.h"

MainMenu::MainMenu(Mage::TextComponent* pOptionsText)
    : m_pOptionsText{ pOptionsText }
{}

void MainMenu::Update()
{
    const auto input = Mage::ServiceLocator::GetInputManager();

    // Choose option
	if (input->CheckKeyboardKey(0x26, Mage::InputState::Down) ||
		input->CheckKeyboardKey(0x28, Mage::InputState::Down) ||
		input->CheckControllerButton(0, Mage::ControllerButton::DPadUp, Mage::InputState::Down) ||
		input->CheckControllerButton(0, Mage::ControllerButton::DPadDown, Mage::InputState::Down) ||
		input->CheckControllerButton(1, Mage::ControllerButton::DPadUp, Mage::InputState::Down) ||
		input->CheckControllerButton(1, Mage::ControllerButton::DPadDown, Mage::InputState::Down) ||
		input->CheckControllerButton(2, Mage::ControllerButton::DPadUp, Mage::InputState::Down) ||
		input->CheckControllerButton(2, Mage::ControllerButton::DPadDown, Mage::InputState::Down) ||
		input->CheckControllerButton(3, Mage::ControllerButton::DPadUp, Mage::InputState::Down) ||
		input->CheckControllerButton(3, Mage::ControllerButton::DPadDown, Mage::InputState::Down))
	{
		switch (m_SelectedOption)
		{
			case SettingsManager::GameMode::SinglePlayer:
			{
				SwitchToMultiplayer();
				break;
			}
			case SettingsManager::GameMode::Multiplayer:
			{
				SwitchToSinglePlayer();
				break;
			}
		}
	}

    // Play
    if (input->CheckKeyboardKey(' ', Mage::InputState::Down) ||
		input->CheckControllerButton(0, Mage::ControllerButton::A, Mage::InputState::Down) ||
		input->CheckControllerButton(1, Mage::ControllerButton::A, Mage::InputState::Down) ||
		input->CheckControllerButton(2, Mage::ControllerButton::A, Mage::InputState::Down) ||
		input->CheckControllerButton(3, Mage::ControllerButton::A, Mage::InputState::Down))
    {
		SettingsManager::GetInstance().SetGameMode(m_SelectedOption);
        Mage::SceneManager::GetInstance().LoadScene("Level01");
    }
}

void MainMenu::SwitchToSinglePlayer()
{
	m_SelectedOption = SettingsManager::GameMode::SinglePlayer;
	m_pOptionsText->SetText("> 1 PLAYER\n\n  2 PLAYERS");
}

void MainMenu::SwitchToMultiplayer()
{
	m_SelectedOption = SettingsManager::GameMode::Multiplayer;
	m_pOptionsText->SetText("  1 PLAYER\n\n> 2 PLAYERS");
}
