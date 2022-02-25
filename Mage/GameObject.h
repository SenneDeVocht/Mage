#pragma once
#include "Transform.h"
#include <algorithm>
#include <typeinfo>
#include "Component.h"

class GameObject final
{
public:
	GameObject(const std::string& name);
	~GameObject() = default;

	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;


	void Update();
	void FixedUpdate();
	void DestroyMarkedObjects();
	void Render() const;

	// Name
	const std::string& GetName() const { return m_Name; }
	void SetName(const std::string& name) { m_Name = name; }

	// Transform
	Transform* GetTransform() const { return m_pTransform.get(); }

	// Components
	void AddComponent(std::shared_ptr<Component> component);

	template<typename typeToFind>
	typeToFind* GetComponentByType() const;

	// Children - Parent
	void AddChild(std::shared_ptr<GameObject> child);
	std::vector<GameObject*> GetChildren() const;
	void SetParent(GameObject* pParentGameObject) { m_pParentGameObject = pParentGameObject; }
	GameObject* GetParent() const { return m_pParentGameObject; }

	// Destruction
	void Destroy() { m_IsMarkedForDestroy = true; }
	bool IsMarkedForDestroy() const { return m_IsMarkedForDestroy; }

private:
	// Name
	std::string m_Name;

	// Transform and other components
	std::shared_ptr<Transform> m_pTransform = std::make_shared<Transform>();
	std::vector<std::shared_ptr<Component>> m_Components;

	// Children - Parent
	std::vector<std::shared_ptr<GameObject>> m_Children;
	GameObject* m_pParentGameObject = nullptr;

	// Destruction
	bool m_IsMarkedForDestroy = false;
};

template<typename typeToFind>
typeToFind* GameObject::GetComponentByType() const
{
	auto it = std::find_if(
		m_Components.begin(),
		m_Components.end(),
		[](std::shared_ptr<Component> component)
		{
			return typeid(typeToFind) == typeid(*component);
		});

	if (it == m_Components.end())
		return nullptr;

	return dynamic_cast<typeToFind*>((*it).get());
}