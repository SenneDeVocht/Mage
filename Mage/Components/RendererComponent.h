#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
	class Font;
	class Texture2D;

	class RendererComponent : public Component
	{
	public:
		explicit RendererComponent() = default;
		explicit RendererComponent(const std::string& filename);

		void Render() const override;

		void SetTexture(std::shared_ptr<Texture2D> pTexture);
		void SetTexture(const std::string& filename);

	private:
		std::shared_ptr<Texture2D> m_pTexture;
	};
}