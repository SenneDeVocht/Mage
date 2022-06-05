#include "BurgerTime/BurgerTimePCH.h"
#include "ScoreUI.h"

#include "BurgerTime/Managers/ScoreManager.h"
#include "Mage/Components/TextComponent.h"
#include "Mage/Scenegraph/GameObject.h"

ScoreUI::ScoreUI(ScoreManager* scoreManager)
	: m_pScoreManager{ scoreManager }
{}

void ScoreUI::Initialize()
{
	m_pScoreText = GetGameObject()->GetComponent<Mage::TextComponent>();
}

void ScoreUI::Update()
{
	m_pScoreText->SetText(std::to_string(m_pScoreManager->GetScore()));
}
