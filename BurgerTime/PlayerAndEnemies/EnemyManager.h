#pragma once
#include "Mage/Components/Component.h"

namespace Mage
{
    class Transform;
}

class PeterPepper;
class Observer;
class GameManager;
class Level;
class Enemy;

class EnemyManager final : public Mage::Component
{
public:
	EnemyManager(GameManager* pGameManager, Level* level);
    ~EnemyManager() override = default;

    EnemyManager(const EnemyManager& other) = delete;
    EnemyManager(EnemyManager&& other) noexcept = delete;
    EnemyManager& operator=(const EnemyManager& other) = delete;
    EnemyManager& operator=(EnemyManager&& other) noexcept = delete;

    void Initialize() override;
    void Update() override;

    void AddPlayer(PeterPepper* pPeterPepper) { m_PeterPeppers.push_back(pPeterPepper); }
    void AddEnemyObserver(Observer* observer);
    void Reset();

private:
    void SpawnEnemy();

    std::vector<Observer*> m_EnemyObservers;

    std::vector<glm::vec2> m_SpawnPositions;
    Level* m_pLevel;
    std::vector<PeterPepper*> m_PeterPeppers{};

    std::vector<Enemy*> m_SpawnedEnemies{};
    const size_t m_MaxSpawnedEnemies = 5;
    const float m_SpawnInterval{ 5.f };
    float m_TimeSinceLastSpawn;
};

