#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
	class Texture2D;

	class AnimatedSpriteComponent : public Component
	{
	public:
		explicit AnimatedSpriteComponent(std::shared_ptr<Texture2D> pSpritesheet, int numFrames, float secondsPerFrame, float layer = 0.0f);

		void Update() override;
		void DrawProperties() override;
		void Render() const override;

		void SetSpritesheet(std::shared_ptr<Texture2D> pTexture);
		
		void SetNumberOfFrames(int numFrames) { m_NumFrames = numFrames; }
		void SetSecondsPerFrame(float secondsPerFrame) { m_SecondsPerFrame = secondsPerFrame; }

	private:
		std::shared_ptr<Texture2D> m_pSpritesheet{};
		float m_Layer;

		int m_NumFrames{};
		float m_SecondsPerFrame{};

		int m_CurrentFrame{ 0 };
		float m_Timer{ 0 };
	};
}