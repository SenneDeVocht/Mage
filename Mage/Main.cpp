//      _______________________
//     |     |  _  |   __|   __|
//     | | | |     |  |  |   __|
//     |_|_|_|__|__|_____|_____|
//

#include "MagePCH.h"

#if _DEBUG
    // ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Mage.h"

int main(int, char* []) {
    Mage engine;
    engine.Run();

    return 0;
}