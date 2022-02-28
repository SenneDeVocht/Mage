//      _______________________
//     |     |  _  |   __|  ___|
//     | |_| |     |  |  |  _|_
//     |_| |_|__|__|_____|_____|
//

#include "MagePCH.h"

#if _DEBUG
	#include <vld.h>
#endif

#include "Mage.h"

int main(int, char* []) {
    Mage engine;
    engine.Run();

    return 0;
}