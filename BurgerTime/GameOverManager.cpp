#include "BurgerTime/BurgerTimePCH.h"
#include "GameOverManager.h"

#include "Mage/Engine/Timer.h"
#include "Mage/Scenegraph/SceneManager.h"

void GameOverManager::Update()
{
    m_Timer += Mage::Timer::GetInstance().GetDeltaTime();

    if (m_Timer > m_TimeBeforeSkip)
        Mage::SceneManager::GetInstance().LoadScene("MainMenu");
}
