#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
    class Transform;
}

class GameManager;
class Level;
class Enemy;

class EnemyManager final : public Mage::Component
{
public:
	EnemyManager(GameManager* pGameManager, Level* level, Mage::Transform* targetTransform);
    ~EnemyManager() override = default;

    EnemyManager(const EnemyManager& other) = delete;
    EnemyManager(EnemyManager&& other) noexcept = delete;
    EnemyManager& operator=(const EnemyManager& other) = delete;
    EnemyManager& operator=(EnemyManager&& other) noexcept = delete;

    void Initialize() override;
    void Update() override;

    void Reset();

private:
    void SpawnEnemy();

    std::vector<glm::vec2> m_SpawnPositions;
    Level* m_pLevel;
    Mage::Transform* m_pTargetTransform;

    std::vector<Enemy*> m_SpawnedEnemies{};
    const size_t m_MaxSpawnedEnemies = 5;
    const float m_SpawnInterval{ 5.f };
    float m_TimeSinceLastSpawn;
};

