#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
	class CameraComponent : public Component
	{
	public:
		explicit CameraComponent(float width, float height)
			: m_Width{ width }
			, m_Height{ height }
		{}

		void SetWidth(float width) { m_Width = width; }
		void SetHeight(float height) { m_Height = height; }

		float GetWidth() const { return m_Width; }
		float GetHeight() const { return m_Height; }


		void Update() override;

	private:
		float m_Width{};
		float m_Height{};
	};
}