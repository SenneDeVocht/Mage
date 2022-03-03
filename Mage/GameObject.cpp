#include "MagePCH.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"

GameObject::GameObject(const std::string& name, GameObject* parent)
	: m_Name{ name }
    , m_pParentGameObject{ parent }
{
	CreateComponent<Transform>();
}

// this is necessary to use unique pointers of incomplete types
GameObject::~GameObject() = default;

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

const std::string& GameObject::GetName() const
{
    return m_Name;
}

void GameObject::SetName(const std::string& name)
{
    m_Name = name;
}

GameObject* GameObject::CreateChildObject(const std::string& name)
{
	auto child = std::unique_ptr<GameObject>(new GameObject(name, this));
	const auto pChild = child.get();

	m_Children.push_back(std::move(child));

	return pChild;
}

std::vector<GameObject*> GameObject::GetChildren() const
{
	std::vector<GameObject*> vecRaw;

	vecRaw.reserve(m_Children.size());
	std::transform(m_Children.cbegin(), m_Children.cend(), std::back_inserter(vecRaw),
		[](auto& ptr) { return ptr.get(); });

	return vecRaw;
}

GameObject* GameObject::GetParent() const
{
	return m_pParentGameObject;
}

void GameObject::Destroy()
{
    m_IsMarkedForDestroy = true;
}

bool GameObject::IsMarkedForDestroy() const
{
    return m_IsMarkedForDestroy;
}