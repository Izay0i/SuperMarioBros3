# Super Mario Bros. 3 Demo v2.0 (WIP)

![Preview 1](Previews/preview_1.jpg)

## Installation
Only do this if the project isn't running.
+ Download VS2019 [here](https://visualstudio.microsoft.com/vs/)
+ Linker > Input > Additional Dependencies: `d3d10.lib;dxguid.lib;dinput8.lib;`
+ Tools > NuGet Package Manager > Package Manager Console and paste this in `Install-Package Microsoft.DXSDK.D3DX -Version 9.29.952.8`
+ [***IMPORTANT***] Configuration: `Active(Debug)`, Platform: `Win32`

## Controls
Mesen default keyboard controls:
|Key          |Action                |
|-------------|----------------------|
|<kbd>U</kbd> |Select (intro)        |
|<kbd>I</kbd> |Start (intro)         |
|<kbd>A</kbd> |Move left             |
|<kbd>D</kbd> |Move right            |
|<kbd>S</kbd> |Crouch/Duck           |
|<kbd>J</kbd> |Attack/Hold shell/Run |
|<kbd>K</kbd> |Jump/Select (map)     |

## Scenes Available
+ Intro
+ Overworld map
+ Stage 1-1

## Credits
+ 125scratch, Jermungandr for the Mario & Luigi sprites.
+ Fleepa, Superjustinbros for the enemies sprites.
+ JigglyPuffGirl for the goal roulette sprites.
+ Malak for the world 1 map tileset.
+ DarkBowser100, Watcher for the HUD & font sprites.
+ Fleepa for misc. sprites.
+ rotodisco for the stage tileset.

## That's all
![Adios](Previews/dante_adios.gif)