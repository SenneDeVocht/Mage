#include "BurgerTime/BurgerTimePCH.h"
#include "PeterPepper.h"

#include "BurgerTime/GameManager.h"
#include "BurgerTime/PlayerAndEnemies/PlayerMovement.h"
#include "BurgerTime/Level.h"

#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Scenegraph/Scene.h"
#include "Mage/Components/AnimatedSpriteComponent.h"
#include "Mage/Components/BoxColliderComponent.h"
#include "Mage/Components/RigidBodyComponent.h"
#include "Mage/Components/Transform.h"
#include "Mage/Engine/ServiceLocator.h"
#include "Mage/Engine/Timer.h"
#include "Mage/Input/InputManager.h"
#include "Mage/ResourceManagement/ResourceManager.h"

PeterPepper::PeterPepper(int playerIndex, Level* pLevel, const std::shared_ptr<Mage::SpriteAnimation>& pVictory, const std::shared_ptr<Mage::SpriteAnimation>& pDeath)
	: m_PlayerIndex{ playerIndex }
	, m_pLevel{ pLevel}
	, m_pVictory{ pVictory }
	, m_pDeath{ pDeath }
{}

void PeterPepper::Initialize()
{
	auto spawnPos = m_pLevel->GetPlayerSpawnPosition();
	spawnPos.y += 3 / 16.f;
	GetGameObject()->GetTransform()->SetWorldPosition(spawnPos);

	m_pMovement = GetGameObject()->GetComponent<PlayerMovement>();
	m_pAnimatedSprite = GetGameObject()->GetComponent<Mage::AnimatedSpriteComponent>();

	const auto& resourceManager = Mage::ResourceManager::GetInstance();
	m_pPepperSprayRightAnimation = std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("PeterPepper/PepperRight.png", 16), 4, 0.1f, false);
	m_pPepperSprayLeftAnimation  = std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("PeterPepper/PepperLeft.png",  16), 4, 0.1f, false);
	m_pPepperSprayUpAnimation    = std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("PeterPepper/PepperUp.png",    16), 4, 0.1f, false);
	m_pPepperSprayDownAnimation  = std::make_shared<Mage::SpriteAnimation>(resourceManager.LoadTexture("PeterPepper/PepperDown.png",  16), 4, 0.1f, false);
}

void PeterPepper::Update()
{
	// Death
    if (m_State == State::Dead)
    {
        m_DeathTimer += Mage::Timer::GetInstance().GetDeltaTime();

		if (m_DeathTimer >= m_DeathDuration)
			m_DeadAndReady = true;
    }

	// Victory
	if (m_State == State::Victory)
	{
		if (m_pAnimatedSprite->GetAnimation() != m_pVictory.get())
			m_pAnimatedSprite->SetAnimation(m_pVictory);
	}

	// Spray pepper
	const auto input = Mage::ServiceLocator::GetInputManager();
	if (!m_SprayingPepper && m_State == State::Alive && m_PepperCount > 0 &&
		(input->CheckKeyboardKey(0x20, Mage::InputState::Down) && m_PlayerIndex == 0 ||
		input->CheckControllerButton(0, Mage::ControllerButton::A, Mage::InputState::Down) && m_PlayerIndex == 1 ||
		input->CheckControllerButton(1, Mage::ControllerButton::A, Mage::InputState::Down) && m_PlayerIndex == 1 ||
		input->CheckControllerButton(2, Mage::ControllerButton::A, Mage::InputState::Down) && m_PlayerIndex == 1 ||
		input->CheckControllerButton(3, Mage::ControllerButton::A, Mage::InputState::Down) && m_PlayerIndex == 1))
	{
		SprayPepper();
	}

	// Stop spraying pepper
	if (m_SprayingPepper)
	{
		m_PepperSprayTimer += Mage::Timer::GetInstance().GetDeltaTime();

		if (m_PepperSprayTimer >= m_PepperSprayDuration)
		{
			StopSprayingPepper();
		}
	}
}

void PeterPepper::OnTriggerEnter(Mage::BoxColliderComponent* other)
{
	if (m_State == State::Alive && other->GetGameObject()->GetTag() == "Enemy")
	{
		Die();
	}
}

void PeterPepper::SprayPepper()
{
	m_SprayingPepper = true;
	--m_PepperCount;

	const auto& dir = m_pMovement->GetDirection();
	glm::vec2 pepperPos = GetGameObject()->GetTransform()->GetWorldPosition();
	std::shared_ptr<Mage::SpriteAnimation> animation;

	if (dir.x == 1)
	{
		pepperPos.x += 1;
		animation = m_pPepperSprayRightAnimation;
	}
	else if (dir.x == -1)
	{
		pepperPos.x -= 1;
		animation = m_pPepperSprayLeftAnimation;
	}
	else if (dir.y == 1)
	{
		pepperPos.y += 1;
		animation = m_pPepperSprayUpAnimation;
	}
	else if (dir.y == -1)
	{
		pepperPos.y -= 1;
		animation = m_pPepperSprayDownAnimation;
	}

	m_pPepperSpray = GetGameObject()->GetScene()->CreateChildObject("PepperSpray");
	m_pPepperSpray->SetTag("PepperSpray");
	m_pPepperSpray->GetTransform()->SetWorldPosition(pepperPos);
	m_pPepperSpray->CreateComponent<Mage::RigidBodyComponent>(Mage::RigidBodyComponent::BodyType::Static);
	m_pPepperSpray->CreateComponent<Mage::BoxColliderComponent>(glm::vec2{ 1, 1 }, glm::vec2{ 0, 0 }, 0.f, true);
	const auto animationComponent = m_pPepperSpray->CreateComponent<Mage::AnimatedSpriteComponent>();
	animationComponent->SetAnimation(animation);
}

void PeterPepper::StopSprayingPepper()
{
	m_SprayingPepper = false;
	m_PepperSprayTimer = 0;
	m_pPepperSpray->Destroy();
	m_pPepperSpray = nullptr;
}

void PeterPepper::Die()
{
	m_State = State::Dead;
	
	m_pMovement->SetEnabled(false);
	m_pAnimatedSprite->SetAnimation(m_pDeath);
}

void PeterPepper::Reset()
{
	auto spawnPos = m_pLevel->GetPlayerSpawnPosition();
	spawnPos.y += 3 / 16.f;
	GetGameObject()->GetTransform()->SetWorldPosition(spawnPos);

	m_State = State::Alive;
	m_pMovement->SetEnabled(true);
	m_DeathTimer = 0;
	m_DeadAndReady = false;
}

void PeterPepper::StartVictory()
{
	m_State = State::Victory;
	m_pMovement->SetEnabled(false);
}

