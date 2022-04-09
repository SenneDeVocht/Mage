#include "Mage/MagePCH.h"
#include "CameraComponent.h"

#include "Transform.h"
#include "Mage/Input/InputManager.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Engine/Timer.h"

void Mage::CameraComponent::Update()
{
	float deltaTime = Mage::Timer::GetInstance().GetDeltaTime();


	if (Mage::InputManager::GetInstance().CheckButton(0, ControllerButton::DPadLeft, InputState::Hold))
		GetGameObject()->GetTransform()->Translate(-10 * deltaTime, 0);


	if (Mage::InputManager::GetInstance().CheckButton(0, ControllerButton::DPadRight, InputState::Hold))
		GetGameObject()->GetTransform()->Translate(10 * deltaTime, 0);


	if (Mage::InputManager::GetInstance().CheckButton(0, ControllerButton::DPadUp, InputState::Hold))
		GetGameObject()->GetTransform()->Translate(0, 10 * deltaTime);


	if (Mage::InputManager::GetInstance().CheckButton(0, ControllerButton::DPadDown, InputState::Hold))
		GetGameObject()->GetTransform()->Translate(0, -10 * deltaTime);




	if (Mage::InputManager::GetInstance().CheckButton(0, ControllerButton::LeftShoulder, InputState::Hold))
	{
		m_Width *= 1 + deltaTime;
		m_Height *= 1 + deltaTime;
	}
	if (Mage::InputManager::GetInstance().CheckButton(0, ControllerButton::RightShoulder, InputState::Hold))
	{
		m_Width /= 1 + deltaTime;
		m_Height /= 1 + deltaTime;
	}
}
