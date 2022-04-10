#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
	class AnimatedSpriteComponent;
}

class PeterPepper final : public Mage::Component
{
public:
	explicit PeterPepper(Mage::AnimatedSpriteComponent* pIdle, Mage::AnimatedSpriteComponent* pWalkfront, Mage::AnimatedSpriteComponent* pWalkBack,
		Mage::AnimatedSpriteComponent* pWalkLeft, Mage::AnimatedSpriteComponent* pWalkRight);
	~PeterPepper() override = default;

	PeterPepper(const PeterPepper& other) = delete;
	PeterPepper(PeterPepper&& other) = delete;
	PeterPepper& operator=(const PeterPepper& other) = delete;
	PeterPepper& operator=(PeterPepper&& other) = delete;

	void Update() override;

private:
	Mage::AnimatedSpriteComponent* m_pIdle{};
	Mage::AnimatedSpriteComponent* m_pWalkFront{};
	Mage::AnimatedSpriteComponent* m_pWalkBack{};
	Mage::AnimatedSpriteComponent* m_pWalkLeft{};
	Mage::AnimatedSpriteComponent* m_pWalkRight{};
};

