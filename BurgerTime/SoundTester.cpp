#include "BurgerTime/BurgerTimePCH.h"
#include "SoundTester.h"

#include "Mage/Engine/ServiceLocator.h"
#include "Mage/Input/InputManager.h"
#include "Mage/Engine/SoundManager.h"
#include "Mage/ResourceManagement/ResourceManager.h"

SoundTester::SoundTester()
{
	m_pSoundClips = {
		Mage::ResourceManager::GetInstance().LoadSoundClip("Sounds/cbar_hit1.wav"),
		Mage::ResourceManager::GetInstance().LoadSoundClip("Sounds/cbar_hit2.wav")
	};

	std::cout << "[SoundTester] Press space to play sound" << std::endl;
}

void SoundTester::Update()
{
	// SOUND TEST
	//-----------
	if (Mage::ServiceLocator::GetInputManager()->CheckKeyboardKey(0x20, Mage::InputState::Down))
	{
		int r = rand();
		r %= m_pSoundClips.size();

		Mage::ServiceLocator::GetSoundManager()->PlaySound(m_pSoundClips[r].get());
	}
}
