#include "Mage/MagePCH.h"
#include "SceneManager.h"

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Scenegraph/Scene.h"

#include "imgui.h"

void Mage::SceneManager::DrawImGui() const
{
	m_ActiveScene->DrawImGui();
}

void Mage::SceneManager::Update() const
{
	m_ActiveScene->Update();
}

void Mage::SceneManager::FixedUpdate() const
{
	m_ActiveScene->FixedUpdate();
}

void Mage::SceneManager::Render() const
{
	m_ActiveScene->Render();
}

void Mage::SceneManager::RenderGizmos() const
{
	m_ActiveScene->RenderGizmos();
}

void Mage::SceneManager::ChangeSceneGraph()
{
    if (m_NewSceneShouldLoad)
    {
		// make sure wanted scene exists
		assert(m_SceneInstructions.find(m_WantedSceneName) != m_SceneInstructions.end());


		const auto scene = std::shared_ptr<Scene>(new Scene(m_WantedSceneName));
		m_SceneInstructions.at(m_WantedSceneName)(scene.get());
		m_ActiveScene = scene;
		m_NewSceneShouldLoad = false;
    }

	m_ActiveScene->ChangeSceneGraph();
}

void Mage::SceneManager::RegisterScene(const std::string& name, const std::function<void(Scene*)>& instructions)
{
	// Make sure scene doesn't already exist
	assert(m_SceneInstructions.find(name) == m_SceneInstructions.end());

	// Add scene
    m_SceneInstructions.insert(std::make_pair(name, instructions));
}

void Mage::SceneManager::LoadScene(const std::string& sceneName)
{
	m_WantedSceneName = sceneName;
	m_NewSceneShouldLoad = true;
}