#pragma once
#include "../Singleton.h"

#include <functional>

namespace Mage
{
	class Scene;
	class GameObject;

	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		struct SceneCreationInstructions
		{
		    std::string Name;
			std::function<void(Scene*)> CreationFunction;
		};
		
		~SceneManager() override = default;
		
        SceneManager(const SceneManager& other) = delete;
        SceneManager(SceneManager&& other) noexcept = delete;
        SceneManager& operator=(const SceneManager& other) = delete;
        SceneManager& operator=(SceneManager&& other) noexcept = delete;

		void RegisterScene(const SceneCreationInstructions& instructions);
		void LoadScene(int sceneIndex);
		
		void DrawImGui() const;
		void Update() const;
		void FixedUpdate() const;
		void Render() const;
		void RenderGizmos() const;
		void ChangeSceneGraph() const;

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		std::vector<SceneCreationInstructions> m_SceneInstructions;
		std::shared_ptr<Scene> m_ActiveScene{};
	};
}
