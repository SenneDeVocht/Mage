#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
	class TextComponent;
}

class ScoreManager;

class ScoreUI : public Mage::Component
{
public:
	ScoreUI(ScoreManager* scoreManager);

	void Initialize() override;
	void Update() override;

private:
	ScoreManager* m_pScoreManager;
	Mage::TextComponent* m_pScoreText{};
};

