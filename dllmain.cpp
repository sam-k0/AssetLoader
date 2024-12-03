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
    LHCore::pUnregisterModule(gPluginName);
    return YYTK_OK;
}

void SwitchSpriteLoop()
{
    // Iterate over all sprites in enum
    for (int sprite = 0; sprite < 1215; sprite++)
    {
        LHSpriteEnum currentSprite = static_cast<LHSpriteEnum>(sprite);
        // Get name 
        std::string resname = LHSprites::GetSpriteName(currentSprite);
        //Misc::Print("Checking sprite: " + resname);
        // check if meta file exists
        std::string metaPath = Filesys::GetAssetDir() + resname + ".meta";
        if (Filesys::FileExists(metaPath))
        {
            Misc::Print("Override sprite exists for: " + resname, Color::CLR_BRIGHTPURPLE);
            // Read meta file
            std::map<std::string, std::string> data = Filesys::LoadDataFromFile(metaPath);
            int data_imgnum = Filesys::GetDataImageCount(data);
            std::string data_spritesheet = Filesys::GetDataSpritesheet(data);

            // Check if both are valid
            if (data_imgnum > 0 && data_spritesheet != "")
            {
                // To fix an issue related to bboxes, get bbox info first: bbox mode, top, bottom, left, right 
                Assets::BBox bbox = Assets::GetSpriteBBox(currentSprite);

                // Replace sprite
                Assets::SpriteReplace((double)currentSprite, "Assets\\"+data_spritesheet, data_imgnum,
                                        Filesys::GetDataDoRemoveBG(data),
                                        Filesys::GetDataDoSmooth(data),
                                        Filesys::GetDataXOrigin(data), 
                                        Filesys::GetDataYOrigin(data));      

                Assets::SetSpriteBBox(currentSprite, bbox);
                Misc::Print("Set sprite bbox to (l,t,r,b,m)" + std::to_string(bbox.left) + "," +
                    std::to_string(bbox.top) + "," +
                    std::to_string(bbox.right) + "," +
                    std::to_string(bbox.bottom) + "," +
                    std::to_string(bbox.mode), Color::CLR_BRIGHTPURPLE);
            }
            else // Meta file sus
            {
                Misc::Print("Invalid metadata file for sprite: " + resname, Color::CLR_RED);                
                //find reason:
                if (data_imgnum <= 0)
                {
                    Misc::Print("Invalid image count for: " + resname, Color::CLR_RED);
                }
                if (!Filesys::FileExists(data_spritesheet))
                {
                    Misc::Print(Filesys::GetAssetDir() + data_spritesheet + " does not exist", Color::CLR_RED);
                }
                if (data_spritesheet == "")
                {
                    Misc::Print("No spritesheet specified in meta file for sprite: " + resname, Color::CLR_RED);
                }
                continue;
            }
        }
        else
        { // Meta file not found, sprite ignored
            continue;
        }
    }
}

DWORD WINAPI SwitchSprites(HINSTANCE hModule)
{
    while (!LHCore::isCoreReady) // wait for core to be ready to handle callbacks / yytk to be ready
    {
        Sleep(10);
    }

    Misc::Print("Swapping sprites now");
    SwitchSpriteLoop();
    return TRUE;
}


// Entry
DllExport YYTKStatus PluginEntry(
    YYTKPlugin* PluginObject // A pointer to the dedicated plugin object
)
{
    gThisPlugin = PluginObject;
    gThisPlugin->PluginUnload = PluginUnload;

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LHCore::ResolveCore, gThisPlugin, 0, NULL); // Check if the Callback Core Module is loaded, and wait for it to load
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

