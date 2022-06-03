#include "BurgerTime/BurgerTimePCH.h"
#include "ScoreUI.h"

#include "ScoreManager.h"
#include "Mage/Components/TextComponent.h"
#include "Mage/Scenegraph/GameObject.h"

void ScoreUI::Initialize()
{
	m_pScoreText = GetGameObject()->GetComponent<Mage::TextComponent>();
}

void ScoreUI::Update()
{
	m_pScoreText->SetText(std::to_string(ScoreManager::GetInstance().GetScore()));
}
