#include "BurgerTime/BurgerTimePCH.h"
#include "Level.h"

#include "Burger/BurgerIngredient.h"
#include "Mage/Components/SpriteComponent.h"
#include "Mage/Components/TilemapComponent.h"
#include "Mage/Components/Transform.h"
#include "Mage/Components/RigidBodyComponent.h"
#include "Mage/Components/BoxColliderComponent.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/ResourceManagement/ResourceManager.h"

void Level::LoadLevel()
{
	// TILES
	//------
#pragma region tiles
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
				}
			}
		}
	}
#pragma endregion

	// BURGER INGREDIENTS
	//-------------------
#pragma region burgerIngredients
	auto burger = GetGameObject()->CreateChildObject("Burger");
	burger->GetTransform()->SetLocalPosition({ -1.5f, 1.8125f });
	m_Ingredients.push_back(burger->CreateComponent<BurgerIngredient>(this, BurgerIngredient::IngredientType::BunTop));
	burger->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Dynamic, true, 0.f);
	burger->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 2.f, 0.5f }, glm::vec2{ 0.f, 0.f }, 0.f, true);
	burger->SetTag("Ingredient", true);

	burger = GetGameObject()->CreateChildObject("Burger");
	burger->GetTransform()->SetLocalPosition({ -1.5f, -0.1875f });
	m_Ingredients.push_back(burger->CreateComponent<BurgerIngredient>(this, BurgerIngredient::IngredientType::Patty));
	burger->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Dynamic, true, 0.f);
	burger->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 2.f, 0.5f }, glm::vec2{ 0.f, 0.f }, 0.f, true);
	burger->SetTag("Ingredient", true);

	burger = GetGameObject()->CreateChildObject("Burger");
	burger->GetTransform()->SetLocalPosition({ -1.5f, -2.1875f });
	m_Ingredients.push_back(burger->CreateComponent<BurgerIngredient>(this, BurgerIngredient::IngredientType::BunBottom));
	burger->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Dynamic, true, 0.f);
	burger->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 2.f, 0.5f }, glm::vec2{ 0.f, 0.f }, 0.f, true);
	burger->SetTag("Ingredient", true);

	const auto burgerCatcher = GetGameObject()->CreateChildObject("BurgerCatcher");
	burgerCatcher->GetTransform()->SetLocalPosition({ -1.5f, -6.85f });
	burgerCatcher->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Static);
	burgerCatcher->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 2.375f, 0.125f }, glm::vec2{ 0.f, -0.09375f }, 0.f);
	burgerCatcher->CreateComponent<Mage::SpriteComponent>(Mage::ResourceManager::GetInstance().LoadTexture("Level/BurgerCatcher.png", 16));
#pragma endregion
}

bool Level::CanMoveInDirection(const glm::vec2& position, Direction direction, bool isAI) const
{
	switch (direction)
	{
		case Direction::Up:
		{
			// Cant go out of map
			if (position.y > m_NumRows / 2.f - 0.5f + m_PlatformHeight)
				return false;

			int idx = PositionToTileIndex(position);

			// Can only go up if horizontally on ladder
			const float xRelToTile = position.x - TileIndexToPosition(idx).x;
			if (abs(xRelToTile) > m_LadderWidth / 2.f)
				return false;

			// Can go up on all tiletypes except None
			if (m_Tiles[idx] == TileType::None)
				return false;

			// On platform can't go up if past the top of the platform
			if (m_Tiles[idx] == TileType::Platform)
			{
				const float yRelToTile = position.y - TileIndexToPosition(idx).y;
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

			const int idx = PositionToTileIndex(position);

			// Can only go down if horizontally on ladder
			const float xRelToTile = position.x - TileIndexToPosition(idx).x;
			if (abs(xRelToTile) > m_LadderWidth / 2.f)
				return false;

			// Can't go down if not ladder or both underneath
			const float yRelToTile = position.y - TileIndexToPosition(idx).y;
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

			const int idx = PositionToTileIndex(position);

			// Needs to be aligned with platform
			const float yRelToTile = position.y - TileIndexToPosition(idx).y;
			if (yRelToTile > m_MaxAbovePlatform || yRelToTile < -m_MaxBelowPlatform)
				return false;

			// Can't move horizontally if not on platform or both
			if (m_Tiles[idx] != TileType::Platform && m_Tiles[idx] != TileType::Both)
				return false;

			// Tile left needs to be platform or both
			const float xRelToTile = position.x - TileIndexToPosition(idx).x;
			if (xRelToTile < 0 || isAI && xRelToTile < -m_LadderWidth / 2.f)
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

			const int idx = PositionToTileIndex(position);

			// Needs to be aligned with platform
			const float yRelToTile = position.y - TileIndexToPosition(idx).y;
			if (yRelToTile > m_MaxAbovePlatform || yRelToTile < -m_MaxBelowPlatform)
				return false;

			// Can't move horizontally if not on platform or both
			if (m_Tiles[idx] != TileType::Platform && m_Tiles[idx] != TileType::Both)
				return false;

			// Tile right needs to be platform or both
			const float xRelToTile = position.x - TileIndexToPosition(idx).x;
			if (xRelToTile > 0 || isAI && xRelToTile > m_LadderWidth / 2.f)
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
	const int idx = PositionToTileIndex(position);

	const float newYPos = TileIndexToPosition(idx).y + m_PlatformHeight;
	return { position.x, newYPos };
}

glm::vec2 Level::GetNextPlatformDown(const glm::vec2& position) const
{
	const size_t currentIndex = PositionToTileIndex(position);

	for (size_t i = 1; i < m_Tiles.size(); i++)
	{
		const size_t indexToCheck = currentIndex + i * m_NumCols;

		// Could not find platform underneath
		if (indexToCheck >= m_Tiles.size())
			break;

		// return if platform
		if (m_Tiles[indexToCheck] == TileType::Platform ||
			m_Tiles[indexToCheck] == TileType::Both)
		{
			return TileIndexToPosition((int)indexToCheck);
		}
	}

	return position;
}

bool Level::IsCompleted()
{
	return std::all_of(
		m_Ingredients.begin(), m_Ingredients.end(),
		[](BurgerIngredient* ingredient) {
		    return ingredient->IsCollected();
		});
}

glm::vec2 Level::TileIndexToPosition(int index) const
{
	const int tileX = index % m_NumCols;
	const int tileY = index / m_NumCols;

	const glm::vec2 offset = { (m_NumCols - 1) * 1.5f / 2.f, (m_NumRows - 1) / 2.f };
	return { tileX * 1.5f - offset.x, -(tileY - offset.y) };
}

int Level::PositionToTileIndex(const glm::vec2 & position) const
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
