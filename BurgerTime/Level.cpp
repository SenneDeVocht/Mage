#include "BurgerTime/BurgerTimePCH.h"
#include "Level.h"

#include <fstream>
#include <map>

#include "BurgerTime/EventSystem.h"
#include "BurgerTime/GameManager.h"
#include "BurgerTime/Burger/BurgerIngredient.h"

#include "Mage/Components/SpriteComponent.h"
#include "Mage/Components/TilemapComponent.h"
#include "Mage/Components/Transform.h"
#include "Mage/Components/RigidBodyComponent.h"
#include "Mage/Components/BoxColliderComponent.h"
#include "Mage/Engine/ServiceLocator.h"
#include "Mage/Input/InputManager.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/ResourceManagement/ResourceManager.h"

Level::Level(GameManager* pGameManager)
	: m_pGameManager{ pGameManager }
{
	m_pGameManager->RegisterLevel(this);
	LoadLevel(1);
}

void Level::Initialize()
{
	m_pTileMap = GetGameObject()->GetComponent<Mage::TilemapComponent>();
}

void Level::Update()
{
	if (!m_Initialized)
	{
		// FILL TILEMAP
		//-------------
		for (auto it = m_Tiles.begin(); it != m_Tiles.end(); ++it)
		{
			// Narrow
			if (it->first.x % 2 == 0)
			{
				switch (it->second)
				{
					case TileType::Platform:
					{
						m_pTileMap->SetTile(it->first, 0);
						break;
					}
					case TileType::Ladder:
					{
						m_pTileMap->SetTile(it->first, 1);
						break;
					}
					case TileType::Both:
					{
						m_pTileMap->SetTile(it->first, 2);
						break;
					}
				}
			}
			// Wide
			else
			{
				switch (it->second)
				{
					case TileType::Platform:
					{
						m_pTileMap->SetTile(it->first, 3);
						break;
					}
					case TileType::Ladder:
					{
						m_pTileMap->SetTile(it->first, 4);
						break;
					}
					case TileType::Both:
					{
						m_pTileMap->SetTile(it->first, 5);
						break;
					}
				}
			}
		}

		// SPAWN INGREDIENTS
		//------------------
		for (const auto& i : m_IngredientStartPositions)
		{
			SpawnIngredient(i.second, i.first);
		}

		// SPAWN CATCHERS
		//---------------
		for (const auto& pos : m_CatcherPositions)
		{
			SpawnBurgerCatcher(pos);
		}

		m_Initialized = true;
	}
	else
	{
		const bool completed = std::all_of(
			m_Ingredients.begin(), m_Ingredients.end(),
			[](BurgerIngredient* ingredient) {
				return ingredient->IsCollected();
			});

		if (completed || Mage::ServiceLocator::GetInputManager()->CheckKeyboardKey('C', Mage::InputState::Down))
		{
			m_pGameManager->OnLevelCompleted();
		}
	}
}

bool Level::CanMoveInDirection(const glm::vec2& position, Direction direction, bool isAI) const
{
	const auto wPos = GetGameObject()->GetTransform()->GetWorldPosition();

	switch (direction)
	{
		case Direction::Up:
		{
			// Cant go out of map
			if (position.y - wPos.y > m_LargestY + m_PlatformHeight)
				return false;

			const auto tilePos = PositionToTilePosition(position);

			if (m_Tiles.find(tilePos) == m_Tiles.end())
				return false;

			// Can only go up if horizontally on ladder
			const float xRelToTile = position.x - wPos.x - tilePos.x * 1.5f;
			if (abs(xRelToTile) > m_LadderWidth / 2.f)
				return false;
			
			// On platform can't go up if past the top of the platform
			if (m_Tiles.at(tilePos) == TileType::Platform)
			{
				const float yRelToTile = position.y - wPos.y - tilePos.y;
				if (yRelToTile >= m_PlatformHeight)
					return false;
			}

			return true;
		}
		case Direction::Down:
		{
			// Cant go out of map
			if (position.y - wPos.y < m_SmallestY)
				return false;

			const auto tilePos = PositionToTilePosition(position);

			if (m_Tiles.find(tilePos) == m_Tiles.end())
				return false;

			// Can only go down if horizontally on ladder
			const float xRelToTile = position.x - wPos.x - tilePos.x * 1.5f;
			if (abs(xRelToTile) > m_LadderWidth / 2.f)
				return false;

			// Can't go down if not ladder or both underneath
			const float yRelToTile = position.y - wPos.y - tilePos.y;
			if (yRelToTile <= 0 ||
				(m_Tiles.at(tilePos) == TileType::Platform || m_Tiles.at(tilePos) == TileType::Both) && yRelToTile <= m_PlatformHeight)
			{
				const glm::ivec2 posUnder = glm::ivec2(tilePos.x, tilePos.y - 1);
				if (posUnder.y < m_SmallestY)
					return false;

                if (m_Tiles.find(posUnder) == m_Tiles.end())
                    return false;

				if (m_Tiles.at(posUnder) != TileType::Ladder && m_Tiles.at(posUnder) != TileType::Both)
					return false;
			}

			return true;
		}
		case Direction::Left:
		{
			// Cant go out of map
			if (position.x - wPos.x < m_SmallestX * 1.5f + 0.75 || isAI && position.x - wPos.x < m_SmallestX * 1.5f + 0.75 + m_LadderWidth / 2.f)
				return false;

			const auto tilePos = PositionToTilePosition(position);

			if (m_Tiles.find(tilePos) == m_Tiles.end())
				return false;

			// Needs to be aligned with platform
			const float yRelToTile = position.y - wPos.y - tilePos.y;
			if (yRelToTile > m_MaxAbovePlatform || yRelToTile < -m_MaxBelowPlatform)
				return false;

			// Can't move horizontally if not on platform or both
			if (m_Tiles.at(tilePos) != TileType::Platform && m_Tiles.at(tilePos) != TileType::Both)
				return false;

			// Tile left needs to be platform or both
			const float xRelToTile = position.x - wPos.x - tilePos.x * 1.5f;
			if (xRelToTile < 0 || isAI && xRelToTile < m_LadderWidth / 2.f)
			{
				const glm::ivec2 posLeft = glm::ivec2(tilePos.x - 1, tilePos.y);

				if (m_Tiles.find(posLeft) == m_Tiles.end())
					return false;

				if (m_Tiles.at(posLeft) != TileType::Platform && m_Tiles.at(posLeft) != TileType::Both)
					return false;
			}

			return true;
		}
		case Direction::Right:
		{
			// Cant go out of map
			if (position.x - wPos.x > m_LargestX * 1.5f - 0.75 || isAI && position.x - wPos.x > m_LargestX * 1.5f - 0.75 - m_LadderWidth / 2.f)
				return false;

			const auto tilePos = PositionToTilePosition(position);

			if (m_Tiles.find(tilePos) == m_Tiles.end())
				return false;

			// Needs to be aligned with platform
			const float yRelToTile = position.y - wPos.y - tilePos.y;
			if (yRelToTile > m_MaxAbovePlatform || yRelToTile < -m_MaxBelowPlatform)
				return false;

			// Can't move horizontally if not on platform or both
			if (m_Tiles.at(tilePos) != TileType::Platform && m_Tiles.at(tilePos) != TileType::Both)
				return false;

			// Tile right needs to be platform or both
			const float xRelToTile = position.x - wPos.x - tilePos.x * 1.5f;
			if (xRelToTile > 0 || isAI && xRelToTile > -m_LadderWidth / 2.f)
			{
				const glm::ivec2 posRight = glm::ivec2(tilePos.x + 1, tilePos.y);

				if (m_Tiles.find(posRight) == m_Tiles.end())
					return false;

				if (m_Tiles.at(posRight) != TileType::Platform && m_Tiles.at(posRight) != TileType::Both)
					return false;
			}

			return true;
		}
	}

	return false;
}

glm::vec2 Level::SnapToPlatform(const glm::vec2& position) const
{
	const auto wPos = GetGameObject()->GetTransform()->GetWorldPosition();

	const float newYPos = PositionToTilePosition(position).y + wPos.y + m_PlatformHeight;
	return { position.x, newYPos };
}

glm::vec2 Level::GetNextPlatformDown(const glm::vec2& position) const
{
	const auto currentPos = PositionToTilePosition(position);

	for (size_t i = 1; i < m_Tiles.size(); i++)
	{
		const auto posToCheck = glm::ivec2{ currentPos.x, currentPos.y - i };

		// Could not find platform underneath
		if (posToCheck.y < m_SmallestY)
			break;

		// No tile at position
		if (m_Tiles.find(posToCheck) == m_Tiles.end())
            continue;

		// return if platform
		if (m_Tiles.at(posToCheck) == TileType::Platform ||
			m_Tiles.at(posToCheck) == TileType::Both)
		{
			const auto wPos = GetGameObject()->GetTransform()->GetWorldPosition();

			return { posToCheck.x + wPos.x, posToCheck.y + wPos.y };
		}
	}

	return position;
}

glm::vec2 Level::GetPlayerSpawnPosition() const
{
	const auto wPos = GetGameObject()->GetTransform()->GetWorldPosition();
	return { m_PlayerSpawnPosition.x * 1.5f + wPos.x, m_PlayerSpawnPosition.y + wPos.y };
}

std::vector<glm::vec2> Level::GetEnemySpawnPositions() const
{
	const auto wPos = GetGameObject()->GetTransform()->GetWorldPosition();

	std::vector<glm::vec2> positions;
	for (const auto& spawnPos : m_EnemySpawnPositions)
	{
		positions.emplace_back(glm::vec2{ spawnPos.x * 1.5f + wPos.x, spawnPos.y + wPos.y });
	}

	return positions;
}

void Level::LoadLevel(int level)
{
	// Delete previous
	if (m_Initialized)
	{
		m_SmallestX = 0;
		m_SmallestY = 0;
		m_LargestX = 0;
		m_LargestY = 0;

		for (const auto i : m_Ingredients)
		{
			i->GetGameObject()->Destroy();
		}

		for (const auto c : m_Catchers)
		{
			c->Destroy();
		}

		m_pTileMap->EraseAll();

		m_Tiles.clear();

		m_Ingredients.clear();
		m_Catchers.clear();
		m_IngredientStartPositions.clear();
		m_CatcherPositions.clear();
		m_EnemySpawnPositions.clear();

		m_Initialized = false;
	}

	// Load new
	std::ifstream inputFile(Mage::ResourceManager::GetInstance().GetDataRoot() + "Level" + std::to_string(level) + ".txt");

	if (!inputFile.is_open())
		return;

	std::string line;
	while (std::getline(inputFile, line))
	{
		// make sure line isn't whitespace
		if (line.empty())
			continue;

		// [str type] [int posX] [int posY]

		std::string type = line.substr(0, line.find(" "));
		int posX = std::stoi(line.substr(line.find(" ") + 1, line.find(" ", line.find(" ") + 1)));
		int posY = std::stoi(line.substr(line.find(" ", line.find(" ") + 1) + 1));

		if (type == "platform" ||
			type == "ladder" ||
			type == "both")
		{
			if (posX < m_SmallestX)
				m_SmallestX = (float)posX;
			if (posX > m_LargestX)
				m_LargestX = (float)posX;
			if (posY < m_SmallestY)
				m_SmallestY = (float)posY;
			if (posY > m_LargestY)
				m_LargestY = (float)posY;

			if (type == "platform")
				m_Tiles[{ posX, posY }] = TileType::Platform;
			if (type == "ladder")
				m_Tiles[{ posX, posY }] = TileType::Ladder;
			if (type == "both")
				m_Tiles[{ posX, posY }] = TileType::Both;
		}

		if (type == "player")
			m_PlayerSpawnPosition = { posX, posY };

		if (type == "enemy")
			m_EnemySpawnPositions.emplace_back(glm::ivec2{ posX, posY });

		if (type == "catcher")
			m_CatcherPositions.emplace_back(glm::ivec2{ posX, posY });

		if (type == "bunTop")
			m_IngredientStartPositions.emplace_back(std::make_pair(glm::ivec2{ posX, posY }, BurgerIngredient::IngredientType::BunTop));
		if (type == "bunBottom")
			m_IngredientStartPositions.emplace_back(std::make_pair(glm::ivec2{ posX, posY }, BurgerIngredient::IngredientType::BunBottom));
		if (type == "cheese")
			m_IngredientStartPositions.emplace_back(std::make_pair(glm::ivec2{ posX, posY }, BurgerIngredient::IngredientType::Cheese));
		if (type == "patty")
			m_IngredientStartPositions.emplace_back(std::make_pair(glm::ivec2{ posX, posY }, BurgerIngredient::IngredientType::Patty));
		if (type == "salad")
			m_IngredientStartPositions.emplace_back(std::make_pair(glm::ivec2{ posX, posY }, BurgerIngredient::IngredientType::Salad));
		if (type == "tomato")
			m_IngredientStartPositions.emplace_back(std::make_pair(glm::ivec2{ posX, posY }, BurgerIngredient::IngredientType::Tomato));
	}

	inputFile.close();

	m_SmallestX *= 1.5f;
	m_LargestX *= 1.5f;
}

void Level::AddBurgerObserver(Observer* pObserver)
{
	m_BurgerObservers.push_back(pObserver);
}

glm::ivec2 Level::PositionToTilePosition(const glm::vec2& position) const
{
	const auto wPos = GetGameObject()->GetTransform()->GetWorldPosition();
	
	int tileY = (int)round(position.y - wPos.y);

	// alternating tile width of 1 and 2 units
	int tileX = (int)floor(abs(position.x - wPos.x) / 3) * 2;

	if (fmod(abs(position.x - wPos.x), 3.f) < 0.5f)
        tileX += 0;
	else if (fmod(abs(position.x - wPos.x), 3.f) < 2.5f)
        tileX += 1;
    else
        tileX += 2;
	
	return { signbit(position.x - wPos.x) ? -tileX : tileX, tileY };
}

void Level::SpawnBurgerCatcher(const glm::ivec2& position)
{
	const auto burgerCatcher = GetGameObject()->CreateChildObject("BurgerCatcher");
	burgerCatcher->GetTransform()->SetLocalPosition({ position.x * 1.5f, position.y + 0.15f });
	burgerCatcher->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Static);
	burgerCatcher->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 2.375f, 0.125f }, glm::vec2{ 0.f, -0.09375f }, 0.f);
	burgerCatcher->CreateComponent<Mage::SpriteComponent>(Mage::ResourceManager::GetInstance().LoadTexture("Level/BurgerCatcher.png", 16));

	m_Catchers.push_back(burgerCatcher);
}

void Level::SpawnIngredient(BurgerIngredient::IngredientType type, const glm::ivec2& position)
{
	const auto burger = GetGameObject()->CreateChildObject("Burger");
	burger->GetTransform()->SetLocalPosition({ position.x * 1.5f, position.y - 3 / 16.f });
	const auto ingredient = burger->CreateComponent<BurgerIngredient>(this, type);
	m_Ingredients.push_back(ingredient);
	burger->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Dynamic, true, 0.f);
	burger->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 2.f, 0.5f }, glm::vec2{ 0.f, 0.f }, 0.f, true);
	burger->SetTag("Ingredient", true);

	for (const auto o : m_BurgerObservers)
	{
		ingredient->AddObserver(o);
	}
}
