#pragma once
#include "Component.h"
#include "Observer.h"

class PointsDisplayComponent : public Component, public Observer
{
public:
	void Notify(Component* component, Event event) override;
};