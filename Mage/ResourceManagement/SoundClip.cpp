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

void Mage::SoundClip::Play(bool loop)
{
    m_Channel = Mix_PlayChannel(-1, m_pSample, loop ? -1 : 0);
}

void Mage::SoundClip::Stop()
{
	if (m_Channel != -1)
		Mix_HaltChannel(m_Channel);
}