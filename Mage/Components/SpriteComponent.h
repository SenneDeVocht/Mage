#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
	class Texture2D;

	class SpriteComponent : public Component
	{
	public:
		explicit SpriteComponent() = default;
		explicit SpriteComponent(std::shared_ptr<Texture2D> pTexture);

		void DrawProperties() override;
		void Render() const override;

		void SetTexture(std::shared_ptr<Texture2D> pTexture);

	private:
		std::shared_ptr<Texture2D> m_pTexture;
	};
}