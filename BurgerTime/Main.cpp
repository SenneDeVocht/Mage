#include "BurgerTime/BurgerTimePCH.h"

#if _DEBUG
	#include <vld.h>
#endif

#include "Mage/Engine/GameSettings.h"
#include "BurgerTime.h"

int main(int, char* [])
{
    // Settings
    Mage::GameSettings::GetInstance().SetWindowSize({ 240 * 4, 240 * 4 });
    Mage::GameSettings::GetInstance().SetWindowTitle("Burger Time");

    // Run
    BurgerTime burgerTime;
    burgerTime.Run();

    return 0;
}