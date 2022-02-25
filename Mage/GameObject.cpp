#include "MagePCH.h"
#include "GameObject.h"
#include "Component.h"

GameObject::GameObject(const std::string& name)
	: m_Name{ name }
{
	// Init Transform
	m_pTransform->SetGameObject(this);
}

void GameObject::Update()
{
	// Update components
	for (const auto& pComponent : m_Components)
	{
		pComponent->Update();
	}

	// Update children
	for (const auto& pChild : m_Children)
	{
		pChild->Update();
	}
}

void GameObject::FixedUpdate()
{
	// Update Components
	for (const auto& pComponent : m_Components)
	{
		pComponent->FixedUpdate();
	}

	// Update children
	for (const auto& pChild : m_Children)
	{
		pChild->FixedUpdate();
	}
}

void GameObject::DestroyMarkedObjects()
{
	// Destroy marked objects
	const auto pos = std::remove_if(m_Children.begin(), m_Children.end(),
		[](const auto& o) { return o->IsMarkedForDestroy(); });

	m_Children.erase(pos, m_Children.end());

	// Destroy marked components
	const auto pos2 = std::remove_if(m_Components.begin(), m_Components.end(),
		[](const auto& c) { return c->IsMarkedForDestroy(); });

	m_Components.erase(pos2, m_Components.end());

	// Call on remaining objects
	for (auto& object : m_Children)
	{
		object->DestroyMarkedObjects();
	}
}

void GameObject::Render() const
{
	// Render Components
	for (const auto& pComponent : m_Components)
	{
		pComponent->Render();
	}

	// Render children
	for (const auto& pChild : m_Children)
	{
		pChild->Render();
	}
}

void GameObject::AddComponent(std::shared_ptr<Component> component)
{
	m_Components.push_back(component);
	component->SetGameObject(this);
}

void GameObject::AddChild(std::shared_ptr<GameObject> child)
{
	m_Children.push_back(child);
	child->SetParent(this);
}

std::vector<GameObject*> GameObject::GetChildren() const
{
	std::vector<GameObject*> vecRaw;

	vecRaw.reserve(m_Children.size());
	std::transform(m_Children.cbegin(), m_Children.cend(), std::back_inserter(vecRaw),
		[](auto& ptr) { return ptr.get(); });

	return vecRaw;
}
