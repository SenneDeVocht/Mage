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

	const bool cantKeepMovingInCurrentDirection = m_CurrentDirection.x == 1 && !m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Right)
		|| m_CurrentDirection.x == -1 && !m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Left)
		|| m_CurrentDirection.y == 1 && !m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Up)
		|| m_CurrentDirection.y == -1 && !m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Down);

	// MOVE
	//-----
    if (currentTile != m_LastIntersection)
    {
	    glm::ivec2 wantedDirection{ 0, 0 };
	    if (targetPos.x > ownPos.x && m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Right))
		    wantedDirection.x = 1;
        if (targetPos.x < ownPos.x && m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Left))
		    wantedDirection.x = -1;
        if (targetPos.y > ownPos.y && m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Up))
		    wantedDirection.y = 1;
	    if (targetPos.y < ownPos.y && m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Down))
		    wantedDirection.y = -1;

	    if (wantedDirection.y != 0 && m_CurrentDirection.y == 0)
	    {
		    m_CurrentDirection = { 0, wantedDirection.y};
			m_LastIntersection = currentTile;
	    }
        else if (wantedDirection.x != 0 && m_CurrentDirection.x == 0)
        {
		    m_CurrentDirection = { wantedDirection.x, 0 };
			GetGameObject()->GetTransform()->SetLocalPosition(m_pLevel->SnapToPlatform(ownPos));
			m_LastIntersection = currentTile;
        }
	    else if (cantKeepMovingInCurrentDirection) // Cant move in current direction and also not in wanted direction
	    {
			// Try to find any possible direction

		    if (m_CurrentDirection.x == 1)
		    {
			    if (m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Up))
			    {
				    m_CurrentDirection = { 0, 1 };
					m_LastIntersection = currentTile;
			    }
				else if (m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Down))
				{
					m_CurrentDirection = { 0, -1 };
					m_LastIntersection = currentTile;
				}

				else
					assert(false); // got stuck
		    }
			else if (m_CurrentDirection.x == -1)
			{
				if (m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Up))
				{
					m_CurrentDirection = { 0, 1 };
					m_LastIntersection = currentTile;
				}

				else if (m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Down))
				{
					m_CurrentDirection = { 0, -1 };
					m_LastIntersection = currentTile;
				}

				else
					assert(false); // got stuck
			}
			else if (m_CurrentDirection.y == 1)
			{
				if (m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Left))
				{
					m_CurrentDirection = { -1, 0 };
					m_LastIntersection = currentTile;
				}

				else if (m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Right))
				{
					m_CurrentDirection = { 1, 0 };
					m_LastIntersection = currentTile;
				}

				else
					assert(false); // got stuck
			}
			else if (m_CurrentDirection.y == -1)
			{
				if (m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Left))
				{
					m_CurrentDirection = { -1, 0 };
					m_LastIntersection = currentTile;
				}

				else if (m_pLevel->CanMoveInDirection(ownPos, Level::Direction::Right))
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