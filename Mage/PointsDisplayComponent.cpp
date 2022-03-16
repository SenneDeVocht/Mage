#include "MagePCH.h"
#include "PointsDisplayComponent.h"

#include "PeterPepper.h"
#include "TextComponent.h"

void PointsDisplayComponent::Notify(Component* component, Event event)
{
    switch (event)
    {
    case Event::PointsChanged:
        PeterPepper* peter = dynamic_cast<PeterPepper*>(component);
        if (peter == nullptr)
            return;

        int points = peter->GetPoints();

        m_pGameObject->GetComponentByType<TextComponent>()->SetText("POINTS: " + std::to_string(points));
        break;
    }
}
