#include "MagePCH.h"
#include "GameObject.h"
#include "Component.h"

GameObject::GameObject()
{
	// Init Transform
	m_pTransform->SetGameObject(this);
}

void GameObject::Update()
{
	// Update components
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->Update();
	}

	// Update children
	for (const auto& pChild : m_pChildren)
	{
		pChild->Update();
	}
}

void GameObject::FixedUpdate()
{
	// Update Components
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->FixedUpdate();
	}

	// Update children
	for (const auto& pChild : m_pChildren)
	{
		pChild->FixedUpdate();
	}
}

void GameObject::Render() const
{
	// Render Components
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->Render();
	}

	// Render children
	for (const auto& pChild : m_pChildren)
	{
		pChild->Render();
	}
}

void GameObject::AddComponent(std::shared_ptr<Component> component)
{
	m_pComponents.push_back(component);
	component->SetGameObject(this);
}

void GameObject::AddChild(std::shared_ptr<GameObject> child)
{
	m_pChildren.push_back(child);
	child->SetParent(this);
}