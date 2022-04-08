#include "BurgerTime/BurgerTimePCH.h"
#include "FpsCounterComponent.h"

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/TextComponent.h"
#include "Mage/Engine/Timer.h"

void FpsCounterComponent::Update()
{
	float dt = Mage::Timer::GetInstance().GetDeltaTime();
	m_TimeSinceLastUpdate += dt;
	m_FramesSinceLastUpdate++;

    if (m_TimeSinceLastUpdate >= m_TimeBetweenUpdates)
    {
        // Update text
		float fps = m_FramesSinceLastUpdate / m_TimeSinceLastUpdate;
		m_Text = "FPS: " + std::to_string(int(fps));

		// Send text to TextComponent
		auto pTextComponent = m_pGameObject->GetComponentByType<Mage::TextComponent>();
		if (pTextComponent != nullptr)
			pTextComponent->SetText(m_Text);

		// Reset
		m_TimeSinceLastUpdate = 0;
		m_FramesSinceLastUpdate = 0;
	}
}