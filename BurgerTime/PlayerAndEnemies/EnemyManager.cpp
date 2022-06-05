#include "BurgerTime/BurgerTimePCH.h"
#include "EnemyManager.h"

#include "BurgerTime/Managers/GameManager.h"
#include "BurgerTime/PlayerAndEnemies/EnemyMovement.h"
#include "BurgerTime/PlayerAndEnemies/Enemy.h"
#include "BurgerTime/Level.h"

#include "Mage/Components/RigidBodyComponent.h"
#include "Mage/Components/BoxColliderComponent.h"
#include "Mage/Components/AnimatedSpriteComponent.h"
#include "Mage/Components/Transform.h"

#include "Mage/Engine/Timer.h"
#include "Mage/ResourceManagement/ResourceManager.h"
#include "Mage/Scenegraph/GameObject.h"

EnemyManager::EnemyManager(GameManager* pGameManager, Level* level)
	: m_pLevel{ level }
    , m_TimeSinceLastSpawn{ m_SpawnInterval }
{
    pGameManager->RegisterEnemyManager(this);
}

void EnemyManager::Initialize()
{
	m_SpawnPositions = m_pLevel->GetEnemySpawnPositions();
}

void EnemyManager::Update()
{
    m_TimeSinceLastSpawn += Mage::Timer::GetInstance().GetDeltaTime();

    if (m_TimeSinceLastSpawn >= m_SpawnInterval && 
        m_SpawnedEnemies.size() < m_MaxSpawnedEnemies)
    {
        SpawnEnemy();
        m_TimeSinceLastSpawn = 0.0f;
    }

    if (m_SpawnedEnemies.empty())
        return;

    // Remove dead enemies
    const auto pos = std::remove_if(m_SpawnedEnemies.begin(), m_SpawnedEnemies.end(),
        [&](const Enemy* e)
        {
            return e->IsDead();
        }
    );

    m_SpawnedEnemies.erase(pos, m_SpawnedEnemies.end());
}

void EnemyManager::AddEnemyObserver(Observer* observer)
{
	m_EnemyObservers.push_back(observer);
}

void EnemyManager::Reset()
{
    for (auto& e : m_SpawnedEnemies)
    {
        e->GetGameObject()->Destroy();
    }

    m_SpawnedEnemies.clear();
    m_TimeSinceLastSpawn = m_SpawnInterval;

    m_SpawnPositions = m_pLevel->GetEnemySpawnPositions();
}

void EnemyManager::SpawnEnemy()
{
	if (m_SpawnPositions.empty())
		return;

    // Pick random spawn position
    const auto spawnPosition = m_SpawnPositions[rand() % m_SpawnPositions.size()];

    // Pick random type
    const int type = rand() % 3;
    std::string name;
    switch (type)
    {
        case 0:
        {
            name = "MrHotDog";
            break;
        }
        case 1:
        {
            name = "MrPickle";
            break;
        }
        case 2:
        {
            name = "MrEgg";
            break;
        }
    }
    
    // Create enemy
    const auto& resourceManager = Mage::ResourceManager::GetInstance();

    const auto enemyObject = GetGameObject()->CreateChildObject(name);
    enemyObject->SetTag("Enemy");
    enemyObject->GetTransform()->SetLocalPosition(spawnPosition);

    enemyObject->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Dynamic, true, 0.f);
    enemyObject->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 0.5f, 0.5f }, glm::vec2{ 0.f, -0.25f }, 0.f, true);
    enemyObject->CreateComponent<Mage::AnimatedSpriteComponent>();
    const auto mrHotDogMovement = enemyObject->CreateComponent<EnemyMovement>(m_pLevel, m_PeterPeppers,
        std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("Enemies/" + name + "/WalkFront.png", 16), 2, 0.1f),
        std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("Enemies/" + name + "/WalkBack.png", 16), 2, 0.1f),
        std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("Enemies/" + name + "/WalkLeft.png", 16), 2, 0.1f),
        std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("Enemies/" + name + "/WalkRight.png", 16), 2, 0.1f));
    const auto enemy = enemyObject->CreateComponent<Enemy>(mrHotDogMovement,
        std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("Enemies/" + name + "/Stunned.png", 16), 2, 0.1f),
        std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("Enemies/" + name + "/Death.png", 16), 4, 0.1f, false));

    m_SpawnedEnemies.push_back(enemy);
}
