#pragma once
class Component;

class Observer
{
public:
    enum class Event
    {
        PlayerDied
    };

    virtual void Notify(Component* component, Event event) = 0;
};

