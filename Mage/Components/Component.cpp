#include "Mage/MagePCH.h"
#include "Component.h"

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