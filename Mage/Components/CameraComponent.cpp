#include "Mage/MagePCH.h"
#include "CameraComponent.h"

#include "imgui.h"
#include "Mage/ImGui/ImGuiHelper.h"

void Mage::CameraComponent::DrawProperties()
{
	Mage::ImGuiHelper::Component("Camera Component", this, &m_ShouldBeEnabled, [&]()
	{
		ImGui::DragFloat2("Size", &m_Size.x, 0.1f);
	});
}
