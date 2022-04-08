#pragma once
#include "Observer.h"

class Subject
{
public:
	void AddObserver(Observer* observer);
	void RemoveObserver(Observer* observer);
	void Notify(Mage::Component* component, Observer::Event event);

private:
	std::vector<Observer*> m_Observers;
};

