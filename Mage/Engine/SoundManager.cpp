#include "Mage/MagePCH.h"
#include "SoundManager.h"

#include <SDL_mixer.h>
#include <mutex>
#include <queue>
#include <thread>

#include "Mage/ResourceManagement/SoundClip.h"

void Mage::DebugSDLSoundManager::PlaySound(SoundClip* pSoundClip, float volume)
{
    std::cout << "[SDL Sound Manager] Playing sound." << std::endl;
    m_pActualSoundManager->PlaySound(pSoundClip, volume);
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
                // Wait for a sound to be played or for the thread to be stopped
                std::unique_lock<std::mutex> lock(m_SoundQueueMutex);
                m_SoundQueueCV.wait(lock, [&]() { return !m_SoundsToPlay.empty() || m_StopThread.load(); });

                // Check if thread needs to be stopped
                if (m_StopThread.load())
                    break;

                // Play the sound
                const auto clip = m_SoundsToPlay.front().first;
                const float volume = m_SoundsToPlay.front().second;

                if (!clip->IsLoaded())
                    clip->Load();

                clip->SetVolume(volume);
                clip->Play();

                // Remove the sound from the queue
                m_SoundsToPlay.pop();
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

    void PlaySound(SoundClip* pSoundClip, float volume = 1.0f)
    {
        std::lock_guard<std::mutex> lock(m_SoundQueueMutex);
        m_SoundsToPlay.push(std::make_pair(pSoundClip, volume));
        m_SoundQueueCV.notify_all();
    }

private:
    std::queue<std::pair<SoundClip*, float>> m_SoundsToPlay;
    std::thread m_SoundThread;
    std::mutex m_SoundQueueMutex;
    std::condition_variable m_SoundQueueCV;
    std::atomic<bool> m_StopThread;
};

Mage::SDLSoundManager::SDLSoundManager()
    : m_pImpl(std::make_unique<SDLSoundManagerImpl>())
{}

Mage::SDLSoundManager::~SDLSoundManager() = default;

void Mage::SDLSoundManager::PlaySound(SoundClip* pSoundClip, float volume)
{
    m_pImpl->PlaySound(pSoundClip, volume);
}

