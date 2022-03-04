#pragma once

#include <algorithm>
#include <typeinfo>
#include "Scene.h"

class Component;
class Transform;

class GameObject final
{
	friend GameObject* Scene::CreateObject(const std::string& name);

public:
	~GameObject();

	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;
	
	void Update();
	void FixedUpdate();
	void DestroyMarkedObjects();
	void Render() const;

	// Name
	const std::string& GetName() const;
	void SetName(const std::string& name);

	// Components
	template<typename componentType, typename... argTypes>
	componentType* CreateComponent(argTypes&&... args);

	template<typename typeToFind>
	typeToFind* GetComponentByType() const;

	Transform* GetTransform() const;

	// Children - Parent
	GameObject* CreateChildObject(const std::string& name);
	std::vector<GameObject*> GetChildren() const;
	GameObject* GetParent() const;

	// Destruction
	void Destroy();
	bool IsMarkedForDestroy() const;

private:
	GameObject(const std::string& name, GameObject* parent);
	
	std::string m_Name;
	
	//Transform* m_pTransform;
	std::vector<std::unique_ptr<Component>> m_Components;
	Transform* m_pTransform;
	
	std::vector<std::unique_ptr<GameObject>> m_Children;
	GameObject* m_pParentGameObject = nullptr;
	
	bool m_IsMarkedForDestroy = false;
};

template<typename componentType, typename... argTypes>
componentType* GameObject::CreateComponent(argTypes&&... args)
{
	// Can't create more than 1 transform
	if (typeid(componentType) == typeid(Transform) && GetComponentByType<Transform>() != nullptr)
		return nullptr;

	// Create component and add it
	auto component = std::unique_ptr<componentType>(new componentType(args...));
	component->SetGameObject(this);
    const auto pComponent = component.get();
	
	m_Components.push_back(std::move(component));

	return pComponent;
}

template<typename typeToFind>
typeToFind* GameObject::GetComponentByType() const
{
	auto it = std::find_if(
		m_Components.begin(),
		m_Components.end(),
		[](const std::unique_ptr<Component>& component)
		{
			return typeid(typeToFind) == typeid(*component);
		});

	if (it == m_Components.end())
		return nullptr;

	return dynamic_cast<typeToFind*>((*it).get());
}