#include "MagePCH.h"
#include "Component.h"

GameObject* Component::GetGameObject() const
{
    return m_pGameObject;
}

void Component::Destroy()
{
    m_IsMarkedForDestroy = true;
}

bool Component::IsMarkedForDestroy() const
{
    return m_IsMarkedForDestroy;
}

void Component::SetGameObject(GameObject* pGameObject)
{
    m_pGameObject = pGameObject;
}
