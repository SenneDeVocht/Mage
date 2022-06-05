#include "Mage/MagePCH.h"
#include "SoundManager.h"

#include <SDL_mixer.h>
#include <mutex>
#include <queue>
#include <thread>

#include "Mage/ResourceManagement/SoundClip.h"

void Mage::DebugSDLSoundManager::PlaySound(SoundClip* pSoundClip, float volume, bool loops)
{
    std::cout << "[SDL Sound Manager] Playing sound." << std::endl;
    m_pActualSoundManager->PlaySound(pSoundClip, volume, loops);
}

class Mage::SDLSoundManager::SDLSoundManagerImpl
{
public:
    SDLSoundManagerImpl() {
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
        Mix_AllocateChannels(16);

        m_SoundThread = std::thread([&]()
        {
            while (true)
            {
                // Wait for a sound to be played
                std::unique_lock<std::mutex> lock(m_SoundQueueMutex);
                m_SoundQueueCV.wait(lock, [&]() { return !m_SoundQueue.empty() || m_StopThread.load(); });

                // Check if thread needs to be stopped
                if (m_StopThread.load())
                {
                    lock.unlock();
                    break;
                }

                const auto soundToPlay = m_SoundQueue.front();
                m_SoundQueue.pop();

                lock.unlock();

                // Play the sound
                const auto clip = soundToPlay.clip;
                const float volume = soundToPlay.volume;
                const float loops = soundToPlay.loops;

                if (!clip->IsLoaded())
                    clip->Load();

                clip->SetVolume(volume);
                clip->Play(loops);
            }
        });
    }
    ~SDLSoundManagerImpl()
    {
        Mix_CloseAudio();
        m_StopThread.store(true);
        m_SoundQueueCV.notify_all();
        m_SoundThread.join();
    }
    
    SDLSoundManagerImpl(const SDLSoundManagerImpl& other) = delete;
    SDLSoundManagerImpl(SDLSoundManagerImpl&& other) noexcept = delete;
    SDLSoundManagerImpl& operator=(const SDLSoundManagerImpl& other) = delete;
    SDLSoundManagerImpl& operator=(SDLSoundManagerImpl&& other) noexcept = delete;

    void PlaySound(SoundClip* pSoundClip, float volume = 1.0f, bool loops = false)
    {
        std::lock_guard<std::mutex> lock(m_SoundQueueMutex);
        m_SoundQueue.push({ pSoundClip, volume, loops });
        m_SoundQueueCV.notify_all();
    }

private:
    struct Sound
	{
		SoundClip* clip;
		float volume;
        bool loops;
	};

    std::queue<Sound> m_SoundQueue;
    std::thread m_SoundThread;
    std::mutex m_SoundQueueMutex;
    std::condition_variable m_SoundQueueCV;
    std::atomic<bool> m_StopThread;
};

Mage::SDLSoundManager::SDLSoundManager()
    : m_pImpl(std::make_unique<SDLSoundManagerImpl>())
{}

Mage::SDLSoundManager::~SDLSoundManager() = default;

void Mage::SDLSoundManager::PlaySound(SoundClip* pSoundClip, float volume, bool loops)
{
    m_pImpl->PlaySound(pSoundClip, volume, loops);
}

