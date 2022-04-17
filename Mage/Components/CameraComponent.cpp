#include "Mage/MagePCH.h"
#include "CameraComponent.h"

#include "imgui.h"

void Mage::CameraComponent::DrawProperties()
{
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader("Camera Component"))
	{
		ImGui::Checkbox("Enabled", &m_ShouldBeEnabled);

		ImGui::DragFloat2("Size", &m_Size.x, 0.1f);
	}

	ImGui::PopID();
}
