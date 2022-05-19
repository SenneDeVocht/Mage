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

void Mage::SceneManager::ChangeSceneGraph() const
{
	m_ActiveScene->ChangeSceneGraph();
}

void Mage::SceneManager::RegisterScene(const SceneCreationInstructions& instructions)
{
	m_SceneInstructions.emplace_back(instructions);
}

void Mage::SceneManager::LoadScene(int sceneIndex)
{
	const auto scene = std::shared_ptr<Scene>(new Scene(m_SceneInstructions[sceneIndex].Name));
	m_SceneInstructions[sceneIndex].CreationFunction(scene.get());
	m_ActiveScene = scene;
}