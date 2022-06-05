#pragma once

struct Mix_Chunk;

namespace Mage
{
    class SoundClip final
    {
    public:
        SoundClip(const std::string& filePath);
        ~SoundClip();

        bool IsLoaded() const { return m_pSample != nullptr; }
        void Load();

        void Play(bool loop = false);
        void Stop();
        void SetVolume(float volume);

    private:
        std::string m_FilePath;
        Mix_Chunk* m_pSample{ nullptr };
        
        int m_Channel{ -1 };
    };
}
