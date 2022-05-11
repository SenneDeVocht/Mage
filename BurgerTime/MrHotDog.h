#pragma once
#include "Mage/Components/Component.h"

class Level;

namespace Mage
{
    class Transform;
    class AnimatedSpriteComponent;
    class RigidBodyComponent;
}

class MrHotDog final : public Mage::Component
{
public:
	MrHotDog(Level* level, Mage::Transform* target, Mage::AnimatedSpriteComponent* pWalkfront, Mage::AnimatedSpriteComponent* pWalkBack,
		Mage::AnimatedSpriteComponent* pWalkLeft, Mage::AnimatedSpriteComponent* pWalkRight);
	~MrHotDog() override;

	MrHotDog(const MrHotDog& other) = delete;
	MrHotDog(MrHotDog&& other) = delete;
	MrHotDog& operator=(const MrHotDog& other) = delete;
	MrHotDog& operator=(MrHotDog&& other) = delete;

	void Initialize() override;
	void FixedUpdate() override;

private:
	Level* m_pLevel{};
    Mage::Transform* m_pTarget{};

	Mage::RigidBodyComponent* m_pRigidBody{};
	
	Mage::AnimatedSpriteComponent* m_pWalkFront{};
	Mage::AnimatedSpriteComponent* m_pWalkBack{};
	Mage::AnimatedSpriteComponent* m_pWalkLeft{};
	Mage::AnimatedSpriteComponent* m_pWalkRight{};

	const float m_LadderThreshold{ 0.1f };

	int m_LastIntersection{ -1 };
	glm::ivec2 m_CurrentDirection{ 0, 0 };
	float m_Speed{ 2 };
};
