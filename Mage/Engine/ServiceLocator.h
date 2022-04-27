#pragma once

namespace Mage
{
    class Renderer;
    class NullRenderer;

    class InputManager;
    class NullInputManager;

    class SoundManager;
    class NullSoundManager;

    class ServiceLocator final
    {
    public:
        static Renderer* GetRenderer() { return m_pRenderer; }
        static void RegisterRenderer(Renderer* pRenderer);

        static InputManager* GetInputManager() { return m_pInputManager; }
        static void RegisterInputManager(InputManager* pInputManager);

        static SoundManager* GetSoundManager() { return m_pSoundManager; }
        static void RegisterSoundManager(SoundManager* pSoundManager);

    private:
        static Renderer* m_pRenderer;
        static NullRenderer m_NullRenderer;

        static InputManager* m_pInputManager;
        static NullInputManager m_NullInputManager;

        static SoundManager* m_pSoundManager;
        static NullSoundManager m_NullSoundManager;
    };
}
