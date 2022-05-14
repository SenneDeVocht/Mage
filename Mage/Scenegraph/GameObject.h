#pragma once

#include <algorithm>
#include <string>
#include <typeinfo>

namespace Mage
{
	class BoxColliderComponent;
	class Scene;
	class Component;
	class Transform;

	class GameObject final
	{
	public:
		GameObject(const std::string& name, GameObject* parent, Scene* scene);

		void Initialize() const;
		void Update() const;
		void FixedUpdate() const;
		void DrawImGui() const;
		void DrawProperties() const;
		void Render() const;
		void RenderGizmos() const;

		void OnTriggerEnter(BoxColliderComponent* other) const;
		void OnTriggerExit(BoxColliderComponent* other) const;
		void OnCollisionEnter(BoxColliderComponent* other) const;
		void OnCollisionExit(BoxColliderComponent* other) const;

		void ChangeSceneGraph();

		// Name
		const std::string& GetName() const;
		void SetName(const std::string& name);

		// Tag
		const std::string& GetTag() const;
		void SetTag(const std::string& tag, bool changeChildren = false);

		// Components
		template<typename componentType, typename... argTypes>
		componentType* CreateComponent(argTypes&&... args);

		template<typename typeToFind>
		typeToFind* GetComponentByType() const;

		Transform* GetTransform() const;

		// Children - Parent
		GameObject* CreateChildObject(const std::string& name);
		std::vector<GameObject*> GetChildren() const;
		GameObject* GetParent() const;

		// Scene
		Scene* GetScene() const { return m_Scene; }

		// Destruction
		void Destroy();
		bool IsMarkedForDestroy() const;

	private:
		std::string m_Name;
		std::string m_Tag{ "Default" };

		//Transform* m_pTransform;
		std::vector<std::shared_ptr<Component>> m_Components;
		std::vector<std::shared_ptr<Component>> m_ComponentsToAdd;
		Transform* m_pTransform;

		std::vector<std::shared_ptr<GameObject>> m_Children;
		std::vector<std::shared_ptr<GameObject>> m_ChildrenToAdd;
		GameObject* m_pParentGameObject = nullptr;

		Scene* m_Scene;

		bool m_IsMarkedForDestroy = false;
	};

	template<typename componentType, typename... argTypes>
	componentType* GameObject::CreateComponent(argTypes&&... args)
	{
		// Can't create more than 1 transform
		if (typeid(componentType) == typeid(Transform) && GetComponentByType<Transform>() != nullptr)
			return nullptr;

		// Create component and add it
		auto component = std::shared_ptr<componentType>(new componentType(args...));
		component->SetGameObject(this);
		const auto pComponent = component.get();

		m_ComponentsToAdd.push_back(component);

		return pComponent;
	}

	template<typename typeToFind>
	typeToFind* GameObject::GetComponentByType() const
	{
		auto it = std::find_if(
			m_Components.begin(),
			m_Components.end(),
			[](const std::shared_ptr<Component>& component)
			{
				return typeid(typeToFind) == typeid(*component);
			});

		if (it == m_Components.end())
			return nullptr;

		return dynamic_cast<typeToFind*>((*it).get());
	}
}
