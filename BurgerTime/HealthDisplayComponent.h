#pragma once
#include "Mage/Components/Component.h"
#include "Observer.h"

class HealthDisplayComponent : public Mage::Component, public Observer
{
public:
	void Notify(Mage::Component* component, Event event) override;
};
