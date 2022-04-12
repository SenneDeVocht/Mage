#pragma once
namespace Mage
{
	class GameObject;

	class Component
	{
	public:
		virtual ~Component() = default;

		virtual void Initialize() {}
		virtual void DrawImGui() {}
		virtual void Update() {}
		virtual void FixedUpdate() {}
		virtual void Render() const {}

		void ChangeSceneGraph();

		void SetGameObject(GameObject* pGameObject);
		GameObject* GetGameObject() const;

		virtual void Destroy();
		bool IsMarkedForDestroy() const;

		bool IsEnabled() const;
		void SetEnabled(bool enabled);
		
	protected:
		GameObject* m_pGameObject = nullptr;
		bool m_IsMarkedForDestroy = false;

		bool m_IsEnabled{ true };
		bool m_ShouldBeEnabled{ true };
	};
}