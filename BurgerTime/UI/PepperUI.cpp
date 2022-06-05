#include "BurgerTime/BurgerTimePCH.h"
#include "PepperUI.h"

#include "BurgerTime/PlayerAndEnemies/PeterPepper.h"

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/TextComponent.h"

PepperUI::PepperUI(const PeterPepper* pPeterPepper)
    : m_pPeterPepper{ pPeterPepper }
{}

void PepperUI::Initialize()
{
    m_pText = GetGameObject()->GetComponent<Mage::TextComponent>();
}

void PepperUI::Update()
{
    int pepper = m_pPeterPepper->GetPepperCount();
    m_pText->SetText(std::to_string(pepper));
}
