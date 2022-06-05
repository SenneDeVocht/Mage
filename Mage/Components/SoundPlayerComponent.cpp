#include "Mage/MagePCH.h"
#include "SoundPlayerComponent.h"

#include "Mage/Engine/ServiceLocator.h"
#include "Mage/Engine/SoundManager.h"
#include "Mage/ResourceManagement/SoundClip.h"

Mage::SoundPlayerComponent::SoundPlayerComponent(std::shared_ptr<Mage::SoundClip> clip, float volume, bool loop)
	: m_pClip{ clip }
	, m_Volume{ volume }
	, m_Loops{ loop }
{}

void Mage::SoundPlayerComponent::OnDestroy()
{
	m_pClip->Stop();
}

void Mage::SoundPlayerComponent::Play()
{
	ServiceLocator::GetSoundManager()->PlaySound(m_pClip.get(), m_Volume, m_Loops);
}

void Mage::SoundPlayerComponent::Stop()
{
	m_pClip->Stop();
}
