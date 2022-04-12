#pragma once
#include "SceneManager.h"

class b2World;

namespace Mage
{
	class BoxColliderComponent;
	class RigidBodyComponent;
	class GameObject;

	class Scene final
	{
		friend Scene* SceneManager::CreateScene(const std::string& name);

	public:
		explicit Scene(const std::string& name);
		~Scene();

		const std::string& GetName() const;

		void DrawImGui() const;
		void Update() const;
		void FixedUpdate() const;
		void Render() const;
		void ChangeSceneGraph();

		GameObject* CreateObject(const std::string& name);
		std::vector<GameObject*> GetObjects() const;

		template<typename T>
		std::vector<T*> GetAllComponentsByType() const;

		void AddRigidBody(RigidBodyComponent* rigidBody);
		void AddBoxCollider(BoxColliderComponent* boxCollider);

	private:
		std::string m_Name;
		std::vector<std::unique_ptr<GameObject>> m_Objects{};

		static unsigned int m_IdCounter;

		std::unique_ptr<b2World> m_pPhysicsWorld;
	};
}
