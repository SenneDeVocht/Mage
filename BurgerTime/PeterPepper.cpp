#include "BurgerTime/BurgerTimePCH.h"
#include "PeterPepper.h"

#include <functional>

#include "Commands.h"
#include "Mage/Input/InputManager.h"
#include "Subject.h"

PeterPepper::PeterPepper(int controllerIndex)
    : m_Lives{ 3 }
    , m_pLivesSubject{ std::make_unique<Subject>() }
    , m_Points{ 0 }
    , m_pPointsSubject{ std::make_unique<Subject>() }
{
	Mage::InputManager::GetInstance().AddInputAction(new Mage::InputAction{ controllerIndex, Mage::ControllerButton::ButtonX, -1, Mage::InputState::Down, std::make_unique<PeterPepperDieCommand>(this) });
	Mage::InputManager::GetInstance().AddInputAction(new Mage::InputAction{ controllerIndex, Mage::ControllerButton::ButtonY, -1, Mage::InputState::Down, std::make_unique<PeterPepperGainPointsCommand>(this)});
}

PeterPepper::~PeterPepper()
{
}

void PeterPepper::AddLivesObserver(Observer* observer)
{
    m_pLivesSubject->AddObserver(observer);
}

int PeterPepper::GetLives()
{
    return m_Lives;
}

void PeterPepper::Die()
{
    --m_Lives;
    m_pLivesSubject->Notify(this, Observer::Event::PlayerDied);
}

void PeterPepper::AddPointsObserver(Observer* observer)
{
    m_pPointsSubject->AddObserver(observer);
}

int PeterPepper::GetPoints()
{
    return m_Points;
}

void PeterPepper::GainPoints()
{
    m_Points += 100;
    m_pPointsSubject->Notify(this, Observer::Event::PointsChanged);
}