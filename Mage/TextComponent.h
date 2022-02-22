#pragma once
#include "Component.h"

class Font;
class Texture2D;

class TextComponent : public Component
{
public:
	void Update() override;
	void FixedUpdate() override {};
	void Render() const override {};

	void SetText(const std::string& text);
	void SetColor(const SDL_Color& color);
	void SetRenderBlended(bool renderBlended);

	explicit TextComponent(const std::string& text, const std::shared_ptr<Font>& font, const SDL_Color& color, bool renderBlended = true);
	virtual ~TextComponent() = default;
	TextComponent(const Component & other) = delete;
	TextComponent(TextComponent&& other) = delete;
	TextComponent& operator=(const TextComponent& other) = delete;
	TextComponent& operator=(TextComponent&& other) = delete;

private:
	bool m_NeedsUpdate;
	std::string m_Text;
	std::shared_ptr<Font> m_Font;
	SDL_Color m_Color;
	bool m_RenderBlended;
};