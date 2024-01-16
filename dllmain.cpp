// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>

// YYTK is in this now
#include "MyHelper.h"
#include "Assets.h"
#include "LHSprites.h"
// Plugin functionality
#include <fstream>
#include <iterator>
#include "LHCore.h"
#define _CRT_SECURE_NO_WARNINGS


// Unload
YYTKStatus PluginUnload()
{
    return YYTK_OK;
}

void SwitchSpriteLoop()
{

}

DWORD WINAPI SwitchSprites(HINSTANCE hModule)
{
    while (!LHCore::isCoreReady) // wait for core to be ready to handle callbacks / yytk to be ready
    {
        Sleep(10);
    }

    Misc::Print("Swapping sprites now");
    Assets::SpriteReplace(LHSpriteEnum::s_hero_attack_rytsar, "Assets\\amogus_attack.png", 12, true, false, -16.0, 0.0);
    Assets::SpriteReplace(LHSpriteEnum::s_hero_idle_rytsar, "Assets\\amogus_idle.png", 1, true, false, -16.0, 0.0);
    Assets::SpriteReplace(LHSpriteEnum::s_hero_charge_rytsar, "Assets\\amogus_idle.png", 1, true, false, -16.0, 0.0);
    Assets::SpriteReplace(LHSpriteEnum::s_hero_hurt_rytsar, "Assets\\amogus_hurt.png", 1, true, false, -16.0, 0.0);
    Assets::SpriteReplace(LHSpriteEnum::s_hero_warrior, "Assets\\arba_map.png", 4, true, false, -6.0, -6.0);
    return TRUE;
}




// Entry
DllExport YYTKStatus PluginEntry(
    YYTKPlugin* PluginObject // A pointer to the dedicated plugin object
)
{
    gThisPlugin = PluginObject;
    gThisPlugin->PluginUnload = PluginUnload;

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LHCore::ResolveCore, NULL, 0, NULL); // Check if the Callback Core Module is loaded, and wait for it to load
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SwitchSprites, NULL, 0, NULL); // Switch sprites in a different thread as soon as core is loaded
    return YYTK_OK; // Successful PluginEntry.
}



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DllHandle = hModule;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

