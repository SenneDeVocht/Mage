#include "MagePCH.h"
#include "SceneManager.h"

#include "GameObject.h"
#include "Scene.h"
#include "imgui.h"

void SceneManager::Update()
{
	for(auto& scene : m_Scenes)
	{
		scene->Update();
	}

	DisplaySceneGraph();
}

void SceneManager::FixedUpdate()
{
	for (auto& scene : m_Scenes)
	{
		scene->FixedUpdate();
	}
}

void SceneManager::Render()
{
	for (const auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

Scene& SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);
	return *scene;
}

void SceneManager::DisplaySceneGraph() const
{
	ImGui::ShowDemoWindow();

	ImGui::Begin("SceneGraph");

	for (const auto& scene : m_Scenes)
	{
		if (ImGui::TreeNode("Scene"))
		{
			auto objects = scene->GetObjects();
			for (const auto& rootObject : objects)
			{
				if (ImGui::TreeNode(rootObject->GetName().c_str()))
				{
					DisplayChildObjects(rootObject);
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
	}

	ImGui::End();
}

inline void SceneManager::DisplayChildObjects(const GameObject* parent) const
{
	const auto children = parent->GetChildren();

	for (const auto& child : children)
	{
		if (ImGui::TreeNode(child->GetName().c_str()))
		{
			DisplayChildObjects(child);
			ImGui::TreePop();
		}
	}
}
