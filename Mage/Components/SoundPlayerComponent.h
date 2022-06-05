#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
	class SoundClip;

	class SoundPlayerComponent : public Component
	{
	public:
		SoundPlayerComponent(std::shared_ptr<SoundClip> clip, float volume = 1, bool loop = false);

		void OnDestroy() override;

		void Play();
		void Stop();

		void SetVolume(float volume) { m_Volume = volume; }
		float GetVolume() const { return m_Volume; }

		void SetLoop(bool loop) { m_Loops = loop; }
		bool GetLoop() const { return m_Loops; }

	private:
		float m_Volume{ 1 };
		bool m_Loops{ false };

		std::shared_ptr<SoundClip> m_pClip;
	};
}
