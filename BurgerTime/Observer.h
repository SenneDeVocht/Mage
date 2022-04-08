#pragma once
namespace Mage
{
	class Component;
}

class Observer
{
public:
    enum class Event
    {
        PlayerDied,
        PointsChanged
    };

    virtual void Notify(Mage::Component* component, Event event) = 0;
};

