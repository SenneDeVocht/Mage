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
		void DrawImGui() const;
		void Update() const;
		void FixedUpdate() const;
		void Render() const;
		void ChangeSceneGraph();

		GameObject* CreateObject(const std::string& name);
		std::vector<GameObject*> GetObjects() const;

		PhysicsHandler* GetPhysicsHandler() const {	return m_pPhysicsHandler.get(); }

	private:
		std::string m_Name;

		std::unique_ptr<PhysicsHandler> m_pPhysicsHandler{};

		std::vector<std::unique_ptr<GameObject>> m_Objects{};

		static unsigned int m_IdCounter;
	};
}
