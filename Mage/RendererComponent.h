#pragma once
#include "Component.h"

class Font;
class Texture2D;

class RendererComponent : public Component
{
	template<typename componentType, typename... argTypes>
	friend componentType* GameObject::CreateComponent(argTypes&&... args);

public:
	explicit RendererComponent(const std::string& filename);

	virtual ~RendererComponent() = default;
	RendererComponent(const RendererComponent& other) = delete;
	RendererComponent(RendererComponent&& other) = delete;
	RendererComponent& operator=(const RendererComponent& other) = delete;
	RendererComponent& operator=(RendererComponent&& other) = delete;

	void Update() override {};
	void FixedUpdate() override {};
	void Render() const override;

	void SetTexture(std::shared_ptr<Texture2D> pTexture);
	void SetTexture(const std::string& filename);

private:
	explicit RendererComponent() = default;

	std::shared_ptr<Texture2D> m_pTexture;
};