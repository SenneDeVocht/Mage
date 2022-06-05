#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
    class TextComponent;
}

class PeterPepper;

class PepperUI : public Mage::Component
{
public:
	PepperUI(const PeterPepper* pPeterPepper);

	void Initialize() override;
	void Update() override;

private:
	const PeterPepper* m_pPeterPepper;
	Mage::TextComponent* m_pText;
};
