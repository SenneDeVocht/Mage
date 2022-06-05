#pragma once
#include "SettingsManager.h"
#include "Mage/Components/Component.h"

namespace Mage
{
    class TextComponent;
}

class MainMenu : public Mage::Component
{
public:
	explicit MainMenu(Mage::TextComponent* pOptionsText);

	void Update() override;

private:
	void SwitchToSinglePlayer();
	void SwitchToMultiplayer();

	Mage::TextComponent* m_pOptionsText;
	SettingsManager::GameMode m_SelectedOption;
};

