#pragma once
namespace Mage
{
	class GameObject;

	class Component
	{
	public:
		virtual ~Component() = default;

		virtual void DrawImGui() {};
		virtual void Update() {};
		virtual void FixedUpdate() {};
		virtual void Render() const {};

		void SetGameObject(GameObject* pGameObject);
		GameObject* GetGameObject() const;

		virtual void Destroy();
		bool IsMarkedForDestroy() const;
		
	protected:
		GameObject* m_pGameObject = nullptr;
		bool m_IsMarkedForDestroy = false;
	};
}