#pragma once

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
		None,
		Platform,
		Ladder,
		Both
	};

	void Initialize() override;

	bool CanMoveInDirection(const glm::vec2& position, Direction direction, bool isAI = false) const;
	glm::vec2 SnapToPlatform(const glm::vec2& position) const;
	glm::vec2 GetNextPlatformDown(const glm::vec2& position) const;
	int PositionToTileIndex(const glm::vec2& position) const;
	glm::vec2 TileIndexToPosition(int index) const;
	bool IsCompleted();

private:

	std::vector<TileType> m_Tiles;
	const int m_NumCols{ 9 };
	const int m_EquivalentNumSmallCols{ 13 };
	const int m_NumRows{ 10 };

	const float m_LadderWidth{ 0.625f };
	const float m_PlatformHeight{ 3/16.f };

	const float m_MaxAbovePlatform{ 0.4f };
	const float m_MaxBelowPlatform{ 0.f };

	std::vector<BurgerIngredient*> m_Ingredients;
};

