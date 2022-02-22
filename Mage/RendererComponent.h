#pragma once
#include "Component.h"

class Font;
class Texture2D;

class RendererComponent : public Component
{
public:
	explicit RendererComponent() {};

	virtual ~RendererComponent() = default;
	RendererComponent(const RendererComponent& other) = delete;
	RendererComponent(RendererComponent&& other) = delete;
	RendererComponent& operator=(const RendererComponent& other) = delete;
	RendererComponent& operator=(RendererComponent&& other) = delete;

	void Update() override {};
	void FixedUpdate() override {};
	void Render() const override;

	void SetTexture(std::shared_ptr<Texture2D> pTexture) { m_pTexture = pTexture; }
	void SetTexture(const std::string& filename);

private:
	std::shared_ptr<Texture2D> m_pTexture;
};