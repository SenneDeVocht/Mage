#pragma once
#include "Mage\Components\Component.h"

namespace Mage
{
	class SoundClip;
}

class SoundTester : public Mage::Component
{
public:
	SoundTester();
	void Update() override;

private:
	std::vector<std::shared_ptr<Mage::SoundClip>> m_pSoundClips;
};

