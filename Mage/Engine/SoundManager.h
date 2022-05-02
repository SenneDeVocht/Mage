#pragma once

namespace Mage
{
	class SoundClip;

	class SoundManager
	{
	public:
		SoundManager() = default;
        virtual ~SoundManager() = default;

        SoundManager(const SoundManager& other) = delete;
        SoundManager(SoundManager&& other) noexcept = delete;
        SoundManager& operator=(const SoundManager& other) = delete;
        SoundManager& operator=(SoundManager&& other) noexcept = delete;

		virtual void PlaySound(SoundClip* pSoundClip, float volume = 1.0f) = 0;
	};

	class NullSoundManager final : public SoundManager
	{
	public:
		void PlaySound(SoundClip*, float) override {}
	};


	class SDLSoundManager final : public SoundManager
	{
	public:
		SDLSoundManager();
		~SDLSoundManager() override;

        SDLSoundManager(const SDLSoundManager& other) = delete;
        SDLSoundManager(SDLSoundManager&& other) noexcept = delete;
        SDLSoundManager& operator=(const SDLSoundManager& other) = delete;
        SDLSoundManager& operator=(SDLSoundManager&& other) noexcept = delete;

		void PlaySound(SoundClip* pSoundClip, float volume = 1.0f) override;

	private:
		class SDLSoundManagerImpl;
		std::unique_ptr<SDLSoundManagerImpl> m_pImpl;
	};

	class DebugSDLSoundManager final : public SoundManager
	{
	public:
		void PlaySound(SoundClip* pSoundClip, float volume = 1.0f) override;

	private:
		std::unique_ptr<SDLSoundManager> m_pActualSoundManager = std::make_unique<SDLSoundManager>();
	};
}
