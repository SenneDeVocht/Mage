#include "BurgerTime/BurgerTimePCH.h"
#include "LivesUI.h"

#include "BurgerTime/PlayerAndEnemies/PeterPepper.h"

#include "Mage/ResourceManagement/ResourceManager.h"
#include "Mage/Scenegraph/GameObject.h"
#include "Mage/Components/Transform.h"
#include "Mage/Components/SpriteComponent.h"

LivesUI::LivesUI(const PeterPepper* pPeterPepper)
    : m_pPeterPepper{ pPeterPepper }
{}

void LivesUI::Initialize()
{
	for (int i = 0; i < m_pPeterPepper->GetMaxLives(); i++)
	{
		auto lifeObject = GetGameObject()->CreateChildObject("Life");
		lifeObject->GetTransform()->SetLocalPosition({ 0.25f, i * 0.5f + 0.25f });
		lifeObject->CreateComponent<Mage::SpriteComponent>(Mage::ResourceManager::GetInstance().LoadTexture("UI/Life.png", 16));
		m_pLifeObjects.push_back(lifeObject);
	}
}

void LivesUI::Update()
{
    for (size_t i = 0; i < m_pLifeObjects.size(); i++)
    {
        if (i >= m_pPeterPepper->GetLivesLeft())
            m_pLifeObjects[i]->SetEnabled(false);

        else
            m_pLifeObjects[i]->SetEnabled(true);
    }
}
