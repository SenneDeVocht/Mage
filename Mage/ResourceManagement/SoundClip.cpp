#include "Mage/MagePCH.h"
#include "SoundClip.h"

#include <SDL_mixer.h>

Mage::SoundClip::SoundClip(const std::string& filePath)
    : m_FilePath(filePath)
{}

Mage::SoundClip::~SoundClip()
{
    Mix_FreeChunk(m_pSample);
}

void Mage::SoundClip::Load()
{
    m_pSample = Mix_LoadWAV(m_FilePath.c_str());
}

void Mage::SoundClip::SetVolume(float volume)
{
    Mix_VolumeChunk(m_pSample, static_cast<int>(volume * MIX_MAX_VOLUME));
}

void Mage::SoundClip::Play()
{
    Mix_PlayChannel(-1, m_pSample, 0);
}
