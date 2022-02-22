#include "MagePCH.h"
#include "FpsCounterComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "Timer.h"

void FpsCounterComponent::Update()
{
	float dt = Timer::GetInstance().GetDeltaTime();
	m_TimeSinceLastUpdate += dt;
	m_FramesSinceLastUpdate++;

    if (m_TimeSinceLastUpdate >= m_TimeBetweenUpdates)
    {
        // Update text
		float fps = m_FramesSinceLastUpdate / m_TimeSinceLastUpdate;
		m_Text = "FPS: " + std::to_string(int(fps));

		// Send text to TextComponent
		auto pTextComponent = m_pGameObject->GetComponentByType<TextComponent>();
		if (pTextComponent != nullptr)
			pTextComponent->SetText(m_Text);

		// Reset
		m_TimeSinceLastUpdate = 0;
		m_FramesSinceLastUpdate = 0;
	}
}