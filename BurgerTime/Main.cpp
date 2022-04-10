#include "BurgerTime/BurgerTimePCH.h"

#if _DEBUG
	#include <vld.h>
#endif

#include "BurgerTime.h"

int main(int, char* [])
{
    // Run
    BurgerTime burgerTime;
    burgerTime.Run(1024, 1024, "Burger Time");

    return 0;
}