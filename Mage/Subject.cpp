#include "MagePCH.h"
#include "Subject.h"

void Subject::AddObserver(Observer* observer)
{
    m_Observers.push_back(observer);
}

void Subject::Notify(Component* component, Observer::Event event)
{
    for (auto observer : m_Observers)
    {
        observer->Notify(component, event);
    }
}
