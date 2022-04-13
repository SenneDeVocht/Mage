#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
	class AnimatedSpriteComponent;
	class RigidBodyComponent;
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

	void Initialize() override;
	void Update() override;
	void FixedUpdate() override;

	void OnTriggerEnter(Mage::BoxColliderComponent* other) override;
	void OnTriggerExit(Mage::BoxColliderComponent* other) override;

private:
	Mage::RigidBodyComponent* m_pRigidbody{ nullptr };

	Mage::AnimatedSpriteComponent* m_pIdle{};
	Mage::AnimatedSpriteComponent* m_pWalkFront{};
	Mage::AnimatedSpriteComponent* m_pWalkBack{};
	Mage::AnimatedSpriteComponent* m_pWalkLeft{};
	Mage::AnimatedSpriteComponent* m_pWalkRight{};

	glm::vec2 m_InputDir{};
	float m_Speed{ 2 };

	int m_NumPlatformsTouching{ 0 };
	int m_NumLaddersTouching{ 0 };
	Mage::BoxColliderComponent* m_pLastPlatformTouched{};
};

