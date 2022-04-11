#include "Mage/MagePCH.h"
#include "GameObject.h"

#include "Mage/Components/Transform.h"

Mage::GameObject::GameObject(const std::string& name, Mage::GameObject* parent)
	: m_Name{ name }
	, m_pTransform{ CreateComponent<Mage::Transform>() }
	, m_pParentGameObject{ parent }
{
}

// this is necessary to use unique pointers of incomplete types
Mage::GameObject::~GameObject() = default;

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

void Mage::GameObject::ChangeSceneGraph()
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

Mage::Transform* Mage::GameObject::GetTransform() const
{
	return m_pTransform;
}

Mage::GameObject* Mage::GameObject::CreateChildObject(const std::string& name)
{
	auto child = std::make_unique<GameObject>(name, this);
	const auto pChild = child.get();

	m_Children.push_back(std::move(child));

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