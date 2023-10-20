# hihatzlab

## Installation

1. `git submodule update --init --recursive`
2. Install `Visual Studio 2022` and `Projucer`
3. Open `vendor\rubberband\otherbuilds\rubberband-library.vcxproj` in `Visual Studio 2022`
4. Select environment `Debug`, `x64` and compile (Ctrl+Shift+B)
5. Copy `vendor\rubberband\otherbuilds\x64\Debug\rubberband-library.lib` as `Library\rubberband.lib`
6. Run `Projucer`, click `File`, `Open` and load `HiHatzLab.jucer` file
7. Click Visual Studio logo (top center)
8. Select environment `Debug`, `x64` and compile (Ctrl+Shift+B)
9. Copy `Builds\VisualStudio2022\x64\Debug\VST3\HiHatzLab.vst3` as `C:\Program Files\Common Files\VST3\HiHatzLab.vst3`
10. Run `FL Studio`, click `OPTIONS` and `Manage plugins`, then click `Start scan` in top left corner
11. Open `Channel rack`, click `+` sign and `More plugins...`, click `HiHatzLab [64bit]`