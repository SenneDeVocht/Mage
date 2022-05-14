#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
	class Texture2D;

	struct SpriteAnimation
	{
		SpriteAnimation(const std::shared_ptr<Texture2D>& pSpritesheet, int numFrames, float secondsPerFrame)
			: pSpritesheet{ pSpritesheet }
			, NumFrames{ numFrames }
			, SecondsPerFrame{ secondsPerFrame }
		{}

		std::shared_ptr<Texture2D> pSpritesheet{};
		int NumFrames{};
		float SecondsPerFrame{};
	};

	class AnimatedSpriteComponent : public Component
	{
	public:
		explicit AnimatedSpriteComponent(float renderLayer = 0.0f);

		void Update() override;
		void DrawProperties() override;
		void Render() const override;

		void SetAnimation(const std::shared_ptr<SpriteAnimation>& animation) { m_pAnimation = animation; }

	private:
		std::shared_ptr<SpriteAnimation> m_pAnimation{};

		int m_CurrentFrame{ 0 };
		float m_Timer{ 0 };

		float m_RenderLayer;
	};
}