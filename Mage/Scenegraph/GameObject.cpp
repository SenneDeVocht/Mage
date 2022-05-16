#include "Mage/MagePCH.h"
#include "GameObject.h"

#include "Mage/Components/BoxColliderComponent.h"
#include "Mage/Components/Transform.h"
#include "Mage/Scenegraph/Scene.h"

Mage::GameObject::GameObject(const std::string& name, Mage::GameObject* parent, Scene* scene)
	: m_Name{ name }
	, m_pTransform{ CreateComponent<Mage::Transform>() }
	, m_pParentGameObject{ parent }
	, m_Scene{ scene }
{
	if (m_pParentGameObject != nullptr)
		m_Tag = m_pParentGameObject->GetTag();
}

void Mage::GameObject::Initialize() const
{
	// Update components
	for (const auto& pComponent : m_Components)
	{
		if (pComponent->IsEnabled())
			pComponent->Initialize();
	}

	// Update children
	for (const auto& pChild : m_Children)
	{
		pChild->Initialize();
	}
}

void Mage::GameObject::Update() const
{
	// Update components
	for (const auto& pComponent : m_Components)
	{
		if (pComponent->IsEnabled())
			pComponent->Update();
	}

	// Update children
	for (const auto& pChild : m_Children)
	{
		pChild->Update();
	}
}

void Mage::GameObject::FixedUpdate() const
{
	// Update Components
	for (const auto& pComponent : m_Components)
	{
		if (pComponent->IsEnabled())
			pComponent->FixedUpdate();
	}

	// Update children
	for (const auto& pChild : m_Children)
	{
		pChild->FixedUpdate();
	}
}

void Mage::GameObject::DrawImGui() const
{
	// Update components
	for (const auto& pComponent : m_Components)
	{
		if (pComponent->IsEnabled())
			pComponent->DrawImGui();
	}

	// Update children
	for (const auto& pChild : m_Children)
	{
		pChild->DrawImGui();
	}
}

void Mage::GameObject::DrawProperties() const
{
	for (const auto& pComponent : m_Components)
	{
		pComponent->DrawProperties();
	}
}

void Mage::GameObject::Render() const
{
	// Render Components
	for (const auto& pComponent : m_Components)
	{
		if (pComponent->IsEnabled())
			pComponent->Render();
	}

	// Render children
	for (const auto& pChild : m_Children)
	{
		pChild->Render();
	}
}

void Mage::GameObject::RenderGizmos() const
{
	// Render Components
	for (const auto& pComponent : m_Components)
	{
		if (pComponent->IsEnabled())
			pComponent->RenderGizmos();
	}

	// Render children
	for (const auto& pChild : m_Children)
	{
		pChild->RenderGizmos();
	}
}


void Mage::GameObject::OnTriggerEnter(Mage::BoxColliderComponent* other) const
{
	for (const auto& pComponent : m_Components)
	{
		if (pComponent->IsEnabled())
			pComponent->OnTriggerEnter(other);
	}
}

void Mage::GameObject::OnTriggerExit(Mage::BoxColliderComponent* other) const
{
	for (const auto& pComponent : m_Components)
	{
		if (pComponent->IsEnabled())
			pComponent->OnTriggerExit(other);
	}
}

void Mage::GameObject::OnCollisionEnter(Mage::BoxColliderComponent* other) const
{
	for (const auto& pComponent : m_Components)
	{
		if (pComponent->IsEnabled())
			pComponent->OnCollisionEnter(other);
	}
}

void Mage::GameObject::OnCollisionExit(Mage::BoxColliderComponent* other) const
{
	for (const auto& pComponent : m_Components)
	{
		if (pComponent->IsEnabled())
			pComponent->OnCollisionExit(other);
	}
}

void Mage::GameObject::ChangeSceneGraph()
{
	// Add Children
	for (const auto& pChild : m_ChildrenToAdd)
		m_Children.push_back(pChild);

	for (const auto& pChild : m_ChildrenToAdd)
		pChild->Initialize();

	m_ChildrenToAdd.clear();

	// Add Components
	for (const auto& pComponent : m_ComponentsToAdd)
		m_Components.push_back(pComponent);

	for (const auto& pComponent : m_ComponentsToAdd)
		pComponent->Initialize();

	m_ComponentsToAdd.clear();

	// Destroy marked objects
	const auto pos = std::remove_if(m_Children.begin(), m_Children.end(),
		[](const auto& o) { return o->IsMarkedForDestroy(); });

	m_Children.erase(pos, m_Children.end());

	// Destroy marked components
	for (const auto& comp : m_Components)
	{
        if (comp->IsMarkedForDestroy())
            comp->OnDestroy();
	}

	const auto pos2 = std::remove_if(m_Components.begin(), m_Components.end(),
		[](const auto& c) { return c->IsMarkedForDestroy(); });

	m_Components.erase(pos2, m_Components.end());

	// Call on remaining objects
	for (auto& object : m_Children)
	{
		object->ChangeSceneGraph();
	}

	// Call on remaining components
	for (auto& component : m_Components)
	{
		component->ChangeSceneGraph();
	}
}

const std::string& Mage::GameObject::GetName() const
{
	return m_Name;
}

void Mage::GameObject::SetName(const std::string& name)
{
	m_Name = name;
}

const std::string& Mage::GameObject::GetTag() const
{
	return m_Tag;
}

void Mage::GameObject::SetTag(const std::string& tag, bool changeChildren)
{
	m_Tag = tag;

	// change children?
	if (changeChildren)
	{
		for (const auto& pChild : m_Children)
		{
			pChild->SetTag(tag, true);
		}
	}
}

Mage::Transform* Mage::GameObject::GetTransform() const
{
	return m_pTransform;
}

Mage::GameObject* Mage::GameObject::CreateChildObject(const std::string& name)
{
	const auto child = std::shared_ptr<GameObject>(new GameObject(name, this, m_Scene)); // Can't use make_shared because of private constructor
	const auto pChild = child.get();

	m_ChildrenToAdd.push_back(child);

	return pChild;
}

std::vector<Mage::GameObject*> Mage::GameObject::GetChildren() const
{
	std::vector<GameObject*> vecRaw;

	vecRaw.reserve(m_Children.size());
	std::transform(m_Children.cbegin(), m_Children.cend(), std::back_inserter(vecRaw),
		[](auto& ptr) { return ptr.get(); });

	return vecRaw;
}

Mage::GameObject* Mage::GameObject::GetParent() const
{
	return m_pParentGameObject;
}

void Mage::GameObject::Destroy()
{
	m_IsMarkedForDestroy = true;
}

bool Mage::GameObject::IsMarkedForDestroy() const
{
	return m_IsMarkedForDestroy;
}