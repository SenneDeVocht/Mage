#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
	class TextComponent;
}

class ScoreUI : public Mage::Component
{
public:
	void Initialize() override;
	void Update() override;

private:
	Mage::TextComponent* m_pScoreText{};
};

