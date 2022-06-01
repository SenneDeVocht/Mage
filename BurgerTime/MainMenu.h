#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
    class TextComponent;
}

class MainMenu : public Mage::Component
{
public:
	explicit MainMenu(Mage::TextComponent* pPlayText);

	void Update() override;

private:
	Mage::TextComponent* m_pPlayText;
	const float m_BlinkSpeed{ 3.f };
	float m_Timer;
};

