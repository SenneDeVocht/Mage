#include "BurgerTime/BurgerTimePCH.h"
#include "PointsDisplayComponent.h"

#include "PeterPepper.h"
#include "Mage/Components/TextComponent.h"
#include "Mage/Scenegraph/GameObject.h"

void PointsDisplayComponent::Notify(Component* component, Event event)
{
    switch (event)
    {
    case Event::PointsChanged:
        PeterPepper* peter = dynamic_cast<PeterPepper*>(component);
        if (peter == nullptr)
            return;

        int points = peter->GetPoints();

        m_pGameObject->GetComponentByType<Mage::TextComponent>()->SetText("POINTS: " + std::to_string(points));
        break;
    }
}
