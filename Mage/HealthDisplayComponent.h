#pragma once
#include "Component.h"
#include "Observer.h"

class HealthDisplayComponent : public Component, public Observer
{
public:
	void Update() override {};
	void FixedUpdate() override {}
	void Render() const override {}

	void Notify(Component* component, Event event) override;
};
