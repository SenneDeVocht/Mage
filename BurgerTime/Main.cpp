#include "BurgerTime/BurgerTimePCH.h"

#if _DEBUG
	#include <vld.h>
#endif

#include "BurgerTime.h"

int main(int, char* [])
{
    // Run
    BurgerTime burgerTime;
    burgerTime.Run(640, 480, "Burger Time");

    return 0;
}