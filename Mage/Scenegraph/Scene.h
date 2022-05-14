#pragma once
#include "SceneManager.h"

class b2World;

namespace Mage
{
	class PhysicsHandler;
	class GameObject;

	class Scene final
	{
		friend Scene* SceneManager::CreateScene(const std::string& name);

	public:
		explicit Scene(const std::string& name);
		~Scene();

		const std::string& GetName() const;

		void Initialize() const;
		void DrawImGui();
		void Update() const;
		void FixedUpdate() const;
		void Render() const;
		void RenderGizmos() const;
		void ChangeSceneGraph();

		GameObject* CreateChildObject(const std::string& name);
		std::vector<GameObject*> GetObjects() const;

		PhysicsHandler* GetPhysicsHandler() const {	return m_pPhysicsHandler.get(); }

	private:
		void DisplaySceneGraph();
		void DisplayObjectInSceneGraph(GameObject* pObject);
		GameObject* m_pSelectedObjectInHierarchy{};

		std::string m_Name;

		std::unique_ptr<PhysicsHandler> m_pPhysicsHandler{};

		std::vector<std::shared_ptr<GameObject>> m_Objects{};
		std::vector<std::shared_ptr<GameObject>> m_ObjectsToAdd{};

		static unsigned int m_IdCounter;
	};
}
