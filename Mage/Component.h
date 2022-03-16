#pragma once

#include "GameObject.h"

class Component
{
	template<typename componentType, typename... argTypes>
	friend componentType* GameObject::CreateComponent(argTypes&&... args);

public:
	virtual ~Component() = default;
	Component(const Component & other) = delete;
	Component(Component && other) = delete;
	Component& operator=(const Component & other) = delete;
	Component& operator=(Component && other) = delete;

	virtual void Update() {};
	virtual void FixedUpdate() {};
	virtual void Render() const {};

	GameObject* GetGameObject() const;

	virtual void Destroy();
	bool IsMarkedForDestroy() const;


protected:
    explicit Component() = default;
	void SetGameObject(GameObject* pGameObject);

	GameObject* m_pGameObject = nullptr;
	bool m_IsMarkedForDestroy = false;
};
