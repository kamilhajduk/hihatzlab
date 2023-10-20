# hihatzlab

## Installation

1. Install `Visual Studio 2022` and `Projucer`
2. Open `vendor\rubberband\otherbuilds\rubberband-library.vcxproj` in `Visual Studio 2022`
3. Select environment `Debug`, `x64` and compile (Ctrl+Shift+B)
4. Copy `vendor\rubberband\otherbuilds\x64\Debug\rubberband-library.lib` as `Library\rubberband.lib`
5. Run `Projucer`, click `File`, `Open` and load `HiHatzLab.jucer` file
6. Click Visual Studio logo (top center)
7. Select environment `Debug`, `x64` and compile (Ctrl+Shift+B)
8. Copy `Builds\VisualStudio2022\x64\Debug\VST3\HiHatzLab.vst3` as `C:\Program Files\Common Files\VST3\HiHatzLab.vst3`
9. Run `FL Studio`, click `OPTIONS` and `Manage plugins`, then click `Start scan` in top left corner
10. Open `Channel rack`, click `+` sign and `More plugins...`, click `HiHatzLab [64bit]`