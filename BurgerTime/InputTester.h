#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
	class TextComponent;
}

class InputTester final : public Mage::Component
{
public:
	InputTester(Mage::TextComponent* pTextComponent);
	void Update() override;

private:
	Mage::TextComponent* m_TextComponent;
};
