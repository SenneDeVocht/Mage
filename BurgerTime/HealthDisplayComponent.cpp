#include "BurgerTime/BurgerTimePCH.h"
#include "HealthDisplayComponent.h"

#include "PeterPepper.h"
#include "Mage/Components/TextComponent.h"
#include "Mage/Scenegraph/GameObject.h"

void HealthDisplayComponent::Notify(Mage::Component* component, Event event)
{
    switch (event)
    {
        case Event::PlayerDied:
            PeterPepper* peter = dynamic_cast<PeterPepper*>(component);
            if (peter == nullptr)
                return;

            int lives = peter->GetLives();

            m_pGameObject->GetComponentByType<Mage::TextComponent>()->SetText("LIVES: " + std::to_string(lives));
            break;
    }
}
