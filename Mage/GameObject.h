#pragma once
#include "Transform.h"
#include "SceneObject.h"
#include <algorithm>
#include <typeinfo>
#include "Component.h"

class GameObject final : public SceneObject
{
public:
	GameObject();
	~GameObject() = default;

	GameObject(const GameObject & other) = delete;
	GameObject(GameObject && other) = delete;
	GameObject& operator=(const GameObject & other) = delete;
	GameObject& operator=(GameObject && other) = delete;


	void Update() override;
	void FixedUpdate() override;
	void Render() const override;

	// Transform
	Transform* GetTransform() const { return m_pTransform.get(); }

	// Components
	void AddComponent(std::shared_ptr<Component> component);

	template<typename typeToFind>
	typeToFind* GetComponentByType() const;

	// Children - Parent
	void AddChild(std::shared_ptr<GameObject> child);
	void SetParent(GameObject* pParentGameObject) { m_pParentGameObject = pParentGameObject; }
	GameObject* GetParent() const { return m_pParentGameObject; }

private:
	// Transform and other components
	std::shared_ptr<Transform> m_pTransform = std::make_shared<Transform>();
	std::vector<std::shared_ptr<Component>> m_pComponents;

	// Children - Parent
	std::vector<std::shared_ptr<GameObject>> m_pChildren;
	GameObject* m_pParentGameObject = nullptr;
};

template<typename typeToFind>
typeToFind* GameObject::GetComponentByType() const
{
	auto it = std::find_if(
		m_pComponents.begin(),
		m_pComponents.end(),
		[](std::shared_ptr<Component> component)
		{
			return typeid(typeToFind) == typeid(*component);
		});

	if (it == m_pComponents.end())
		return nullptr;

	return dynamic_cast<typeToFind*>((*it).get());
}