#include "BurgerTime/BurgerTimePCH.h"
#include "MrHotDog.h"

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/Transform.h"
#include "Mage/Components/AnimatedSpriteComponent.h"
#include "Mage/Components/RigidBodyComponent.h"
#include "BurgerTime/Level.h"

MrHotDog::MrHotDog(Level* level, Mage::Transform* target, Mage::AnimatedSpriteComponent* pWalkfront, Mage::AnimatedSpriteComponent* pWalkBack,
                   Mage::AnimatedSpriteComponent* pWalkLeft, Mage::AnimatedSpriteComponent* pWalkRight)
	: m_pLevel{ level }
    , m_pTarget{ target }
	, m_pWalkFront{ pWalkfront }
	, m_pWalkBack{ pWalkBack }
	, m_pWalkLeft{ pWalkLeft }
	, m_pWalkRight{ pWalkRight }
{}

MrHotDog::~MrHotDog() = default;

void MrHotDog::Initialize()
{
	m_pRigidBody = GetGameObject()->GetComponentByType<Mage::RigidBodyComponent>();
}

void MrHotDog::FixedUpdate()
{
	const auto ownPos = GetGameObject()->GetTransform()->GetWorldPosition();
	const auto targetPos = m_pTarget->GetWorldPosition();
	const int currentTile = m_pLevel->PositionToTileIndex(ownPos);
	const auto currentTilePos = m_pLevel->TileIndexToPosition(currentTile);

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

				else
					assert(false); // got stuck
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

				else
					assert(false); // got stuck
			}
	    }

	    m_pRigidBody->SetVelocity({ m_CurrentDirection.x * m_Speed, m_CurrentDirection.y * m_Speed });
	}

	// ANIMATION
	//----------
	m_pWalkFront->SetEnabled(false);
	m_pWalkBack->SetEnabled(false);
	m_pWalkLeft->SetEnabled(false);
	m_pWalkRight->SetEnabled(false);

	if (m_CurrentDirection.x == 1)
		m_pWalkRight->SetEnabled(true);
	if (m_CurrentDirection.x == -1)
		m_pWalkLeft->SetEnabled(true);
	if (m_CurrentDirection.y == 1)
		m_pWalkBack->SetEnabled(true);
	if (m_CurrentDirection.y == -1)
		m_pWalkFront->SetEnabled(true);
}