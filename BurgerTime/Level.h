#pragma once

#include <unordered_map>

#include "Burger/BurgerIngredient.h"
#include "Mage/Components/Component.h"

class BurgerIngredient;

class Level : public Mage::Component
{
public:
	enum class Direction
	{
		Up, Down, Left, Right
	};

	enum class TileType
	{
		Platform,
		Ladder,
		Both
	};

	Level(const std::string& filePath);

	void Initialize() override;

	bool CanMoveInDirection(const glm::vec2& position, Direction direction, bool isAI = false) const;
	glm::vec2 SnapToPlatform(const glm::vec2& position) const;
	glm::vec2 GetNextPlatformDown(const glm::vec2& position) const;
	glm::ivec2 PositionToTilePosition(const glm::vec2& position) const;

	glm::vec2 GetPlayerSpawnPosition() const;
	std::vector<glm::vec2> GetEnemySpawnPositions() const;

    bool IsCompleted();

private:
	void SpawnBurgerCatcher(const glm::ivec2& position);
	void SpawnIngredient(BurgerIngredient::IngredientType type, const glm::ivec2& position);

	std::unordered_map<glm::ivec2, TileType> m_Tiles;
	float m_SmallestX{};
	float m_SmallestY{};
	float m_LargestX{};
	float m_LargestY{};

	const float m_LadderWidth{ 0.625f };
	const float m_PlatformHeight{ 3/16.f };

	const float m_MaxAbovePlatform{ 0.4f };
	const float m_MaxBelowPlatform{ 0.f };

	std::vector<BurgerIngredient*> m_Ingredients;

	glm::ivec2 m_PlayerSpawnPosition;
	std::vector<glm::ivec2> m_EnemySpawnPositions;
	std::vector<glm::ivec2> m_CatcherPositions;
	std::vector<std::pair<glm::ivec2, BurgerIngredient::IngredientType>> m_IngredientStartPositions;
};

