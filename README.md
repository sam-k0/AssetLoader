# Sprite Replacer (AssetLoader)

This mod allows you to replace sprites during runtime.
Unlike replacing the sprites in the game files like when using `UndertaleModTool` for example,
you don't need to back-up the game files before modifying them, as the core game files don't get changed on your hard disk.

This makes uninstalling simple: Just remove / rename the `dll` file to something like `dll.d`.

## Installation

To install the mod, follow the basic steps of [installing mods](https://github.com/sam-k0/LoopHero_Mods).
### Quick guide:
- Download [these files](https://github.com/sam-k0/YYToolkit_LoopHero/releases/):
    - dinput8.dll
    - YYToolkit.dll
- Move `dinput8.dll` into the root directory of your game installation (Where `LoopHero.exe` resides).
- Create a folder called `NativeMods` in your root directory
- Move `YYToolkit.dll` into the `NativeMods` folder

You now have installed the modloader!
Don't forget to install the `Callback Core`, as written in the complete guide.
*Let's return back to installing this specific mod..*

Download `LH_AssetLoader.dll` from [here](https://github.com/sam-k0/AssetLoader/releases/).
Create another directory called `autoexec` and move the mod file there

Besides that, you need to create an `Assets` folder in the root game directory.
This is where all your assets / sprites and metadata files will go.

For a quickstart, I made this Amogus asset pack downloadable from the releases as well, which is ready to go.
Just extract it and use the newly created `Assets` folder.

## Metadata files
Each sprite needs a `.png` / `.jpg` file and a `.meta` file.
The image files obviously contain the spritesheet, and the metadata file contains information about it.
Contents of a metadata file look like this:

```ini
SPRITESHEET=amogus_idle.png
IMAGECOUNT=1
XORIG=-16
YORIG=0
REMOVEBG=1
SMOOTH=0
```
The metadata file's name (in this case `s_hero_idle_rytsar.meta`) contains the name of the sprite to replace,
so the mod will look for these at game-startup.

> [!TIP]
> I recommend using UnderTaleModTool to find sprite names and their locations in the game sprite sheets.

The `SPRITESHEET` denotes which image file the game should use.

The `IMAGECOUNT` denotes how many subimages the animation (i.e. Spritesheet) contains.

The `XORIG and YORIG` variables denote where to center the sprite

The `REMOVEBG` denotes if the game should remove the background color of the sprite, which is determined by the top-left pixel color in the image sheet.

The `SMOOTH` variable is useless for pixelart games. It will make your sprites look like shit.

More information can be found in the [GameMaker Docs](https://manual.gamemaker.io/monthly/en/#t=GameMaker_Language%2FGML_Reference%2FAsset_Management%2FSprites%2FSprite_Manipulation%2Fsprite_replace.htm&rhsearch=sprite_replace&rhhlterm=sprite_replace).

## Further tools

The `py` directory contains some scripts to help assembling a spritesheet from loose images.
Note that `pillow` and `tkinter` as well as Python 3.10 are required.
