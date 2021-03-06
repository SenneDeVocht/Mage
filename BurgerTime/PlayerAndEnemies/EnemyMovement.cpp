#include "BurgerTime/BurgerTimePCH.h"
#include "EnemyMovement.h"

#include "PeterPepper.h"

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/Transform.h"
#include "Mage/Components/AnimatedSpriteComponent.h"
#include "Mage/Components/RigidBodyComponent.h"
#include "BurgerTime/Level.h"

EnemyMovement::EnemyMovement(Level* level, const std::vector<PeterPepper*>& peterPeppers, const std::shared_ptr<Mage::SpriteAnimation>& pWalkfront,
                             const std::shared_ptr<Mage::SpriteAnimation>& pWalkBack, const std::shared_ptr<Mage::SpriteAnimation>& pWalkLeft, const std::shared_ptr<Mage::SpriteAnimation>& pWalkRight)
	: m_pLevel{ level }
    , m_PeterPeppers{ peterPeppers }
	, m_pWalkFront{ pWalkfront }
	, m_pWalkBack{ pWalkBack }
	, m_pWalkLeft{ pWalkLeft }
	, m_pWalkRight{ pWalkRight }
{}

void EnemyMovement::Initialize()
{
	m_pRigidBody = GetGameObject()->GetComponent<Mage::RigidBodyComponent>();
	m_pAnimatedSprite = GetGameObject()->GetComponent<Mage::AnimatedSpriteComponent>();
}

void EnemyMovement::FixedUpdate()
{
	const auto ownPos = GetGameObject()->GetTransform()->GetWorldPosition();
	glm::vec2 targetPos{ 0, 0 };
	float closestDistance = std::numeric_limits<float>::max();

	for (auto& pPeterPepper : m_PeterPeppers)
	{
		const auto pPeterPepperPos = pPeterPepper->GetGameObject()->GetTransform()->GetWorldPosition();
		const auto distance = glm::distance(ownPos, pPeterPepperPos);
		if (distance < closestDistance)
		{
			closestDistance = distance;
			targetPos = pPeterPepperPos;
		}
	}

	const auto currentTile = m_pLevel->PositionToTilePosition(ownPos);

	const bool cantKeepMovingInCurrentDirection = m_CurrentDirection.x == 1 && !m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Right, true)
		|| m_CurrentDirection.x == -1 && !m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Left, true)
		|| m_CurrentDirection.y == 1 && !m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Up, true)
		|| m_CurrentDirection.y == -1 && !m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Down, true);

	// MOVE
	//-----
    if (currentTile != m_LastIntersection || cantKeepMovingInCurrentDirection)
    {
	    glm::ivec2 wantedDirection{ 0, 0 };
	    if (targetPos.x > ownPos.x && m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Right, true))
		    wantedDirection.x = 1;
        if (targetPos.x < ownPos.x && m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Left, true))
		    wantedDirection.x = -1;
        if (targetPos.y > ownPos.y && m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Up, true))
		    wantedDirection.y = 1;
	    if (targetPos.y < ownPos.y && m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Down, true))
		    wantedDirection.y = -1;

	    if (wantedDirection.y != 0 && m_CurrentDirection.y != -wantedDirection.y)
	    {
			if (wantedDirection.y != m_CurrentDirection.y)
				m_LastIntersection = currentTile;

		    m_CurrentDirection = { 0, wantedDirection.y};
	    }
        else if (wantedDirection.x != 0 && m_CurrentDirection.x != -wantedDirection.x)
        {
			if (wantedDirection.x != m_CurrentDirection.x)
				m_LastIntersection = currentTile;

		    m_CurrentDirection = { wantedDirection.x, 0 };
			GetGameObject()->GetTransform()->SetLocalPosition(m_pLevel->SnapToPlatform(ownPos));
        }
	    else if (cantKeepMovingInCurrentDirection) // Cant move in current direction and also not in wanted direction
	    {
			// Try to find any possible direction
		    if (m_CurrentDirection.x != 0)
		    {
			    if (m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Up, true))
			    {
				    m_CurrentDirection = { 0, 1 };
					m_LastIntersection = currentTile;
			    }
				else if (m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Down, true))
				{
					m_CurrentDirection = { 0, -1 };
					m_LastIntersection = currentTile;
				}
		    }
			else if (m_CurrentDirection.y != 0)
			{
				if (m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Left, true))
				{
					m_CurrentDirection = { -1, 0 };
					m_LastIntersection = currentTile;
				}
				else if (m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Right, true))
				{
					m_CurrentDirection = { 1, 0 };
					m_LastIntersection = currentTile;
				}
			}
	    }

	    m_pRigidBody->SetVelocity({ m_CurrentDirection.x * m_Speed, m_CurrentDirection.y * m_Speed });
	}

	// ANIMATION
	//----------
	if (m_CurrentDirection.x == 1 && m_pAnimatedSprite->GetAnimation() != m_pWalkRight.get())
		m_pAnimatedSprite->SetAnimation(m_pWalkRight);

	else if (m_CurrentDirection.x == -1 && m_pAnimatedSprite->GetAnimation() != m_pWalkLeft.get())
		m_pAnimatedSprite->SetAnimation(m_pWalkLeft);

	else if (m_CurrentDirection.y == 1 && m_pAnimatedSprite->GetAnimation() != m_pWalkBack.get())
		m_pAnimatedSprite->SetAnimation(m_pWalkBack);

	else if (m_CurrentDirection.y == -1 && m_pAnimatedSprite->GetAnimation() != m_pWalkFront.get())
		m_pAnimatedSprite->SetAnimation(m_pWalkFront);
}

void EnemyMovement::OnDisable()
{
	m_pRigidBody->SetVelocity({ 0, 0 });
}

void EnemyMovement::OnEnable()
{
	m_pRigidBody->SetVelocity({ m_CurrentDirection.x * m_Speed, m_CurrentDirection.y * m_Speed });
}

