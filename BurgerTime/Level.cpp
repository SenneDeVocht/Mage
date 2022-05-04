#include "BurgerTime/BurgerTimePCH.h"
#include "Level.h"

#include "Mage/Components/SpriteComponent.h"
#include "Mage/Components/TilemapComponent.h"
#include "Mage/Components/Transform.h"
#include "Mage/ResourceManagement/ResourceManager.h"
#include "Mage/Scenegraph/GameObject.h"

void Level::LoadLevel()
{

	// Init the stage
	m_Tiles = {
		TileType::Platform, TileType::Platform, TileType::Platform, TileType::Platform, TileType::Platform, TileType::Platform, TileType::Platform, TileType::Platform, TileType::Platform,
		TileType::Ladder,   TileType::None,     TileType::Ladder,   TileType::Ladder,   TileType::Ladder,   TileType::None,     TileType::Ladder,   TileType::None,     TileType::Ladder,
		TileType::Both,     TileType::Platform, TileType::Both,     TileType::Ladder,   TileType::Both,     TileType::Platform, TileType::Both,     TileType::Platform, TileType::Both,
		TileType::None,     TileType::Ladder,   TileType::Both,     TileType::Both,     TileType::Both,     TileType::Ladder,   TileType::Ladder,   TileType::None,     TileType::Ladder,
		TileType::Platform, TileType::Both,     TileType::Both,     TileType::None,     TileType::Ladder,   TileType::Ladder,   TileType::Both,     TileType::Platform, TileType::Both,
		TileType::Ladder,   TileType::Ladder,   TileType::Both,     TileType::Platform, TileType::Both,     TileType::Both,     TileType::Both,     TileType::Ladder,   TileType::None,
		TileType::Ladder,   TileType::Ladder,   TileType::Ladder,   TileType::None,     TileType::Ladder,   TileType::None,     TileType::Both,     TileType::Both,     TileType::Platform,
		TileType::Both,     TileType::Both,     TileType::Both,     TileType::Platform, TileType::Both,     TileType::Platform, TileType::Both,     TileType::Ladder,   TileType::Ladder,
		TileType::Ladder,   TileType::None,     TileType::Ladder,   TileType::None,     TileType::Ladder,   TileType::None,     TileType::Ladder,   TileType::Ladder,   TileType::Ladder,
		TileType::Both,     TileType::Platform, TileType::Both,     TileType::Platform, TileType::Both,     TileType::Platform, TileType::Both,     TileType::Both,     TileType::Both
	};

	auto tilemap = GetGameObject()->GetComponentByType<Mage::TilemapComponent>();

	// Spawn sprites
	for (int tileY = 0; tileY < m_NumRows; tileY++)
	{
		for (int tileX = 0; tileX < m_NumCols; tileX++)
		{
			const int index = tileY * m_NumCols + tileX;

			const int posX = tileX - m_NumCols / 2;
			const int posY = m_NumRows / 2 - tileY;

			// Narrow
			if (tileX % 2 == 0)
			{
				switch (m_Tiles[index])
				{
					case TileType::Platform:
					{
						tilemap->SetTile(glm::ivec2{ posX, posY }, 0);
						break;
					}
					case TileType::Ladder:
					{
						tilemap->SetTile(glm::ivec2{ posX, posY }, 1);
						break;
					}
					case TileType::Both:
					{
						tilemap->SetTile(glm::ivec2{ posX, posY }, 2);
						break;
					}
					default:
					{
						break;
					}
				}
			}
			// Wide
			else
			{
				switch (m_Tiles[index])
				{
					case TileType::Platform:
					{
						tilemap->SetTile(glm::ivec2{ posX, posY }, 3);
						break;
					}
					case TileType::Ladder:
					{
						tilemap->SetTile(glm::ivec2{ posX, posY }, 4);
						break;
					}
					case TileType::Both:
					{
						tilemap->SetTile(glm::ivec2{ posX, posY }, 5);
						break;
					}
					default:
					{
						break;
					}
				}
			}
		}
	}
}

bool Level::CanMoveInDirection(const glm::vec2& position, Direction direction) const
{
	switch (direction)
	{
		case Direction::Up:
		{
			// Cant go out of map
			if (position.y > m_NumRows / 2.f - 0.5f + m_PlatformHeight)
				return false;

			int idx = PositionToIndex(position);

			// Can only go up if horizontally on ladder
			const float xRelToTile = position.x - IndexToPosition(idx).x;
			if (abs(xRelToTile) > m_LadderWidth / 2.f)
				return false;

			// Can go up on all tiletypes except None
			if (m_Tiles[idx] == TileType::None)
				return false;

			// On platform can't go up if past the top of the platform
			if (m_Tiles[idx] == TileType::Platform)
			{
				const float yRelToTile = position.y - IndexToPosition(idx).y;
				if (yRelToTile >= m_PlatformHeight)
					return false;
			}

			return true;
		}
		case Direction::Down:
		{
			// Cant go out of map
			if (position.y < -m_NumRows / 2.f + 0.5f)
				return false;

			const int idx = PositionToIndex(position);

			// Can only go down if horizontally on ladder
			const float xRelToTile = position.x - IndexToPosition(idx).x;
			if (abs(xRelToTile) > m_LadderWidth / 2.f)
				return false;

			// Can't go down if not ladder or both underneath
			const float yRelToTile = position.y - IndexToPosition(idx).y;
			if (yRelToTile <= 0 ||
				(m_Tiles[idx] == TileType::Platform || m_Tiles[idx] == TileType::Both) && yRelToTile <= m_PlatformHeight)
			{
				const int idxUnder = idx + m_NumCols;
				if (idxUnder >= (int)m_Tiles.size())
					return false;

				if (m_Tiles[idxUnder] != TileType::Ladder && m_Tiles[idxUnder] != TileType::Both)
					return false;
			}

			return true;
		}
		case Direction::Left:
		{
			// Cant go out of map
			if (position.x < -m_NumCols * 1.5f / 2.f + 0.75)
				return false;

			const int idx = PositionToIndex(position);

			// Needs to be aligned with platform
			const float yRelToTile = position.y - IndexToPosition(idx).y;
			if (yRelToTile > m_MaxAbovePlatform || yRelToTile < -m_MaxBelowPlatform)
				return false;

			// Can't move horizontally if not on platform or both
			if (m_Tiles[idx] != TileType::Platform && m_Tiles[idx] != TileType::Both)
				return false;

			// Tile left needs to be platform or both
			const float xRelToTile = position.x - IndexToPosition(idx).x;
			if (xRelToTile < 0)
			{
				const int idxLeft = idx - 1;
				if (m_Tiles[idxLeft] != TileType::Platform && m_Tiles[idxLeft] != TileType::Both)
					return false;
			}

			return true;
		}
		case Direction::Right:
		{
			// Cant go out of map
			if (position.x > m_NumCols * 1.5f / 2.f - 0.75)
				return false;

			const int idx = PositionToIndex(position);

			// Needs to be aligned with platform
			const float yRelToTile = position.y - IndexToPosition(idx).y;
			if (yRelToTile > m_MaxAbovePlatform || yRelToTile < -m_MaxBelowPlatform)
				return false;

			// Can't move horizontally if not on platform or both
			if (m_Tiles[idx] != TileType::Platform && m_Tiles[idx] != TileType::Both)
				return false;

			// Tile right needs to be platform or both
			const float xRelToTile = position.x - IndexToPosition(idx).x;
			if (xRelToTile > 0)
			{
				const int idxRight = idx + 1;
				if (m_Tiles[idxRight] != TileType::Platform && m_Tiles[idxRight] != TileType::Both)
					return false;
			}

			return true;
		}
	}

	return false;
}

glm::vec2 Level::SnapToPlatform(const glm::vec2& position) const
{
	const int idx = PositionToIndex(position);

	const float newYPos = IndexToPosition(idx).y + m_PlatformHeight;
	return { position.x, newYPos };
}

glm::vec2 Level::GetNextPlatformDown(const glm::vec2& position) const
{
	const size_t currentIndex = PositionToIndex(position);

	for (size_t i = 1; i < m_Tiles.size(); i++)
	{
		const size_t indexToCheck = currentIndex + i * m_NumCols;

		// Could not find platform underneath
		if (indexToCheck >= m_Tiles.size())
			break;

		if (m_Tiles[indexToCheck] == TileType::Platform ||
			m_Tiles[indexToCheck] == TileType::Both)
		{
			return IndexToPosition((int)indexToCheck);
		}
	}

	return IndexToPosition((int)currentIndex);
}

glm::vec2 Level::IndexToPosition(int index) const
{
	const int tileX = index % m_NumCols;
	const int tileY = index / m_NumCols;

	const glm::vec2 offset = { (m_NumCols - 1) * 1.5f / 2.f, (m_NumRows - 1) / 2.f };
	return { tileX * 1.5f - offset.x, -(tileY - offset.y) };
}

int Level::PositionToIndex(const glm::vec2 & position) const
{
	// Outside of level
	if (abs(position.x) >= m_NumCols * 1.5f / 2 ||
		abs(position.y) >= m_NumRows / 2.f)
	{
		return -1;
	}
	
	const int smallIndexX = int(position.x + m_EquivalentNumSmallCols / 2.f);
	constexpr int smallToNormalLookup[]{ 0, 1, 1, 2, 3, 3, 4, 5, 5, 6, 7, 7, 8 };

	const int indexX = smallToNormalLookup[smallIndexX];
	const int indexY = int(m_NumRows / 2.f - position.y);

	return indexX + indexY * m_NumCols;
}
