#include "MagePCH.h"
#include "FpsCounterComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "Timer.h"

void FpsCounterComponent::Update()
{
	// Update Text
	float fps = 1 / Timer::GetInstance().GetDeltaTime();
	m_Text = "FPS: " + std::to_string(int(fps));

	auto pTextComponent = m_pGameObject->GetComponentByType<TextComponent>();
	if (pTextComponent != nullptr)
		pTextComponent->SetText(m_Text);
}