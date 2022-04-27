#include "Mage/MagePCH.h"
#include "ServiceLocator.h"

#include "Mage/Engine/Renderer.h"
#include "Mage/Input/InputManager.h"
#include "Mage/Engine/SoundManager.h"

Mage::NullRenderer Mage::ServiceLocator::m_NullRenderer;
Mage::Renderer* Mage::ServiceLocator::m_pRenderer = &m_NullRenderer;

void Mage::ServiceLocator::RegisterRenderer(Renderer* pRenderer)
{
    if (pRenderer != m_pRenderer && m_pRenderer != &m_NullRenderer)
        delete m_pRenderer;

    if (pRenderer != nullptr)
        m_pRenderer = pRenderer;
    else
        m_pRenderer = &m_NullRenderer;
}


Mage::NullInputManager Mage::ServiceLocator::m_NullInputManager;
Mage::InputManager* Mage::ServiceLocator::m_pInputManager = &m_NullInputManager;

void Mage::ServiceLocator::RegisterInputManager(InputManager* pInputManager)
{
    if (pInputManager != m_pInputManager && m_pInputManager != &m_NullInputManager)
        delete m_pInputManager;

    if (pInputManager != nullptr)
        m_pInputManager = pInputManager;
    else
        m_pInputManager = &m_NullInputManager;
}


Mage::NullSoundManager Mage::ServiceLocator::m_NullSoundManager;
Mage::SoundManager* Mage::ServiceLocator::m_pSoundManager = &m_NullSoundManager;

void Mage::ServiceLocator::RegisterSoundManager(SoundManager* pSoundManager)
{
    if (pSoundManager != m_pSoundManager && m_pSoundManager != &m_NullSoundManager)
        delete m_pSoundManager;

    if (pSoundManager != nullptr)
        m_pSoundManager = pSoundManager;
    else
        m_pSoundManager = &m_NullSoundManager;
}