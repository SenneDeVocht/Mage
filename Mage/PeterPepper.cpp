#include "MagePCH.h"
#include "PeterPepper.h"

#include <functional>

#include "Command.h"
#include "InputManager.h"
#include "Subject.h"

PeterPepper::PeterPepper()
    : m_Lives{ 3 }
    , m_pLivesSubject{ std::make_unique<Subject>() }
{
    InputManager::GetInstance().AddInputAction(new InputAction{ ControllerButton::ButtonX, InputState::Down, std::make_unique<PeterPepperDieCommand>(this) });
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
