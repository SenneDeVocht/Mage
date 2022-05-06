#include "BurgerTime/BurgerTimePCH.h"
#include "InputTester.h"

#include "Mage/Components/TextComponent.h"
#include "Mage/Engine/ServiceLocator.h"
#include "Mage/Input/InputManager.h"

InputTester::InputTester(Mage::TextComponent* pTextComponent)
	: m_TextComponent{ pTextComponent }
{}

void InputTester::Update()
{
	auto input = Mage::ServiceLocator::GetInputManager();

	const float leftStickX = input->GetControllerAxis(0, Mage::ControllerAxis::LeftStickX);
	const float leftStickY = input->GetControllerAxis(0, Mage::ControllerAxis::LeftStickY);
	const float rightStickX = input->GetControllerAxis(0, Mage::ControllerAxis::RightStickX);
	const float rightStickY = input->GetControllerAxis(0, Mage::ControllerAxis::RightStickY);
	
	const float leftTrigger = input->GetControllerAxis(0, Mage::ControllerAxis::LeftTrigger);
	const float rightTrigger = input->GetControllerAxis(0, Mage::ControllerAxis::RightTrigger);

	m_TextComponent->SetText(
		"LEFT STICK: " + std::to_string(leftStickX) + ", " + std::to_string(leftStickY) + "\n"
		"RIGHT STICK: " + std::to_string(rightStickX) + ", " + std::to_string(rightStickY) + "\n"
		"LEFT TRIGGER: " + std::to_string(leftTrigger) + "\n"
		"RIGHT TRIGGER: " + std::to_string(rightTrigger) + "\n"
	);
}
