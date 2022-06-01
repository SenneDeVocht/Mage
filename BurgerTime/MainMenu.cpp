#include "BurgerTime/BurgerTimePCH.h"
#include "MainMenu.h"

#include "Mage/Components/TextComponent.h"
#include "Mage/Engine/ServiceLocator.h"
#include "Mage/Engine/Timer.h"
#include "Mage/Input/InputManager.h"
#include "Mage/Scenegraph/SceneManager.h"

MainMenu::MainMenu(Mage::TextComponent* pPlayText)
    : m_pPlayText{ pPlayText }
{}

void MainMenu::Update()
{
    // Binking text
    m_Timer += Mage::Timer::GetInstance().GetDeltaTime() * m_BlinkSpeed;
    auto color = static_cast<uint8_t>(abs(std::sin(m_Timer)) * 255);
    m_pPlayText->SetColor({ color, color, color, 255 });

    // Play
    const auto input = Mage::ServiceLocator::GetInputManager();
    if (input->CheckKeyboardKey(' ', Mage::InputState::Down))
        Mage::SceneManager::GetInstance().LoadScene(1);
}
