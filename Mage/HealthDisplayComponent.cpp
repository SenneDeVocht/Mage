#include "MagePCH.h"
#include "HealthDisplayComponent.h"

#include "PeterPepper.h"
#include "TextComponent.h"

void HealthDisplayComponent::Notify(Component* component, Event event)
{
    switch (event)
    {
        case Event::PlayerDied:
            PeterPepper* peter = dynamic_cast<PeterPepper*>(component);
            if (peter == nullptr)
                return;

            int lives = peter->GetLives();

            m_pGameObject->GetComponentByType<TextComponent>()->SetText("LIVES: " + std::to_string(lives));
            break;
    }
}
