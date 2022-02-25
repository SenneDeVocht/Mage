#pragma once

class GameObject;

class Component
{
public:
	explicit Component() = default;
	virtual ~Component() = default;
	Component(const Component & other) = delete;
	Component(Component && other) = delete;
	Component& operator=(const Component & other) = delete;
	Component& operator=(Component && other) = delete;

	virtual void Update() = 0;
	virtual void FixedUpdate() = 0;
	virtual void Render() const = 0;

	void SetGameObject(GameObject* pGameObject) { m_pGameObject = pGameObject; }
	GameObject* GetGameObject() const { return m_pGameObject; }

	void Destroy() { m_IsMarkedForDestroy = true; }
	bool IsMarkedForDestroy() const { return m_IsMarkedForDestroy; }

protected:
	GameObject* m_pGameObject = nullptr;
	bool m_IsMarkedForDestroy = false;
};
