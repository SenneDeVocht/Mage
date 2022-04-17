#pragma once
#include "../Singleton.h"

namespace Mage
{
	class Scene;
	class GameObject;

	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		~SceneManager() override;

		Scene* CreateScene(const std::string& name);
		void SetActiveScene(int sceneIndex);
		
		void DrawImGui() const;
		void Update() const;
		void FixedUpdate() const;
		void Render() const;
		void ChangeSceneGraph() const;

	private:
		friend class Singleton<SceneManager>;
		SceneManager();

		std::vector<std::unique_ptr<Scene>> m_Scenes;
		int m_ActiveScene{};
	};
}
