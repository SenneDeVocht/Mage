#pragma once
#include "Mage/Components/Component.h"

class FpsCounterComponent final : public Mage::Component
{
public:
	void Update() override;

private:
	std::string m_Text;

	const float m_TimeBetweenUpdates = 0.2f;
	int m_FramesSinceLastUpdate = 0;
	float m_TimeSinceLastUpdate = 0.f;
};