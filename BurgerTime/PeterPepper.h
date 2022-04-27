#pragma once
#include "Mage/Components/Component.h"

class Level;

namespace Mage
{
    class SoundClip;
    class RigidBodyComponent;
	class AnimatedSpriteComponent;
}

class PeterPepper final : public Mage::Component
{
public:
	explicit PeterPepper(Level* level, Mage::AnimatedSpriteComponent* pIdle, Mage::AnimatedSpriteComponent* pWalkfront, Mage::AnimatedSpriteComponent* pWalkBack,
		Mage::AnimatedSpriteComponent* pWalkLeft, Mage::AnimatedSpriteComponent* pWalkRight);
	~PeterPepper() override;

	PeterPepper(const PeterPepper& other) = delete;
	PeterPepper(PeterPepper&& other) = delete;
	PeterPepper& operator=(const PeterPepper& other) = delete;
	PeterPepper& operator=(PeterPepper&& other) = delete;

	void Initialize() override;
	void Update() override;
	void FixedUpdate() override;

private:
	Level* m_pLevel{};

	Mage::RigidBodyComponent* m_pRigidBody{};
	
	Mage::AnimatedSpriteComponent* m_pIdle{};
	Mage::AnimatedSpriteComponent* m_pWalkFront{};
	Mage::AnimatedSpriteComponent* m_pWalkBack{};
	Mage::AnimatedSpriteComponent* m_pWalkLeft{};
	Mage::AnimatedSpriteComponent* m_pWalkRight{};

	glm::vec2 m_Input{ 0, 0 };
	float m_Speed{ 2 };

	std::shared_ptr<Mage::SoundClip> m_pSoundClip;
};

