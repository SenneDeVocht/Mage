#pragma once
#include "Component.h"
#include "Observer.h"

class HealthDisplayComponent : public Component, public Observer
{
public:
	void Notify(Component* component, Event event) override;
};
