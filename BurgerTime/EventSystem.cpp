#include "BurgerTime/BurgerTimePCH.h"
#include "EventSystem.h"

void Subject::AddObserver(Observer* observer)
{
    m_Observers.push_back(observer);
}

void Subject::Notify(Event event)
{
    for (auto observer : m_Observers)
    {
        observer->OnNotify(event);
    }
}
