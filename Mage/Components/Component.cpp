#include "Mage/MagePCH.h"
#include "Component.h"

#include "imgui.h"
#include "Mage/ImGui/ImGuiHelper.h"

void Mage::Component::DrawProperties()
{
    Mage::ImGuiHelper::Component("Component", this, &m_ShouldBeEnabled, [&]()
    {
        ImGui::Text("This component does not have DrawProperties() implemented.");
    });
}

Mage::GameObject* Mage::Component::GetGameObject() const
{
    return m_pGameObject;
}

void Mage::Component::Destroy()
{
    m_IsMarkedForDestroy = true;
}

bool Mage::Component::IsMarkedForDestroy() const
{
    return m_IsMarkedForDestroy;
}

void Mage::Component::SetGameObject(GameObject* pGameObject)
{
    if (m_pGameObject == nullptr)
        m_pGameObject = pGameObject;
}

void Mage::Component::ChangeSceneGraph()
{
    m_IsEnabled = m_ShouldBeEnabled;
}

bool Mage::Component::IsEnabled() const
{
    return m_IsEnabled;
}

void Mage::Component::SetEnabled(bool enabled)
{
    m_ShouldBeEnabled = enabled;
}

