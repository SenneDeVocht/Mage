#pragma once
#include "Mage/Components/Component.h"
#include "Observer.h"

class PointsDisplayComponent : public Mage::Component, public Observer
{
public:
	void Notify(Mage::Component* component, Event event) override;
};