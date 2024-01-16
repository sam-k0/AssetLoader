#pragma once
#include <Windows.h>
#include "MyHelper.h"

namespace LHCore {


typedef bool (*CoreReady)();
CoreReady pCoreReady = nullptr;
bool isCoreReady = false;

DWORD WINAPI ResolveCore()
{
    Misc::Print("Importing Core function");
    void* rawCoreReady;

    while (true)
    {
        Sleep(10);
        if (PmGetExported("CoreReady", rawCoreReady) == YYTK_OK)
        {
            pCoreReady = reinterpret_cast<CoreReady>(rawCoreReady);
            if (pCoreReady() == true)
            {
                Misc::Print("Core is present", CLR_GREEN);
                isCoreReady = true;
                return TRUE;
            }
        }
        Misc::Print("Waiting for Core. Did you install LoopHeroCallbackCore.dll?", CLR_RED);
    }
}

}