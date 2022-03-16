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
#include <steam_api.h>

int main(int, char* [])
{
    // Init Steam
    if (!SteamAPI_Init())
    {
        std::cerr << "[Fatal Error] Steam must be running to play this game. (SteamAPI_Init() failed)" << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Succesfully initialized Steam." << std::endl;
    }

    // Run
    Mage engine;
    engine.Run();

    // Shut down Steam
    SteamAPI_Shutdown();

    return 0;
}