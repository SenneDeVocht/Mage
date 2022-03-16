#pragma once
class Component;

class Observer
{
public:
    enum class Event
    {
        PlayerDied,
        PointsChanged
    };

    virtual void Notify(Component* component, Event event) = 0;
};

