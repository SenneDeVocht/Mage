#include "BurgerTime/BurgerTimePCH.h"
#include "HighScoreUI.h"

#include "BurgerTime/SavedScoreManager.h"

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/TextComponent.h"

void HighScoreUI::Initialize()
{
	GetGameObject()->GetComponent<Mage::TextComponent>()->SetText(
		"Highscore: " + std::to_string(SavedScoreManager::GetSavedScore()));
}