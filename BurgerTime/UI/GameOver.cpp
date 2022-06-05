#include "BurgerTime/BurgerTimePCH.h"
#include "GameOver.h"

#include "Mage/Engine/Timer.h"
#include "Mage/Scenegraph/SceneManager.h"

void GameOver::Update()
{
    m_Timer += Mage::Timer::GetInstance().GetDeltaTime();

    if (m_Timer > m_TimeBeforeSkip)
        Mage::SceneManager::GetInstance().LoadScene("MainMenu");
}
