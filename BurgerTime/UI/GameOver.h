#pragma once
#include "Mage/Components/Component.h"

class GameOver final : public Mage::Component
{
public:
	void Update() override;

private:
	const float m_TimeBeforeSkip = 3.0f;
	float m_Timer = 0.0f;
};

