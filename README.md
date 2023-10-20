# HiHatzLab: FL Studio VST3 Plugin

![HiHatzLab](/Docs/screenshot.jpg "HiHatzLab")

HiHatzLab is a powerful open-source VST3 plugin for FL Studio that allows you to manipulate sample pitch, control time signatures, and much more in real-time.
Plugin utilizes the open-source [RubberBand](https://github.com/breakfastquay/rubberband) library for pitch manipulation and [JUCE](https://github.com/juce-framework/JUCE) framework.

## Features ✨

- **Live Pitch Control**: Change sample pitch live by MIDI key.
- **Tempo Control**: Adjust tempo with various options (1/2, 1/4, 1/8 up to 1/128) and triplets (1/3 up to 1/48).
- **Key Mapping**: Intuitive key mapping for easy pitch adjustments.
- **Drag & Drop**: Easily load samples by dragging and dropping files.
- **Presets**: Import and export your favorite settings.
- **Sound Customization**: Band-pass filter, ADSR (Attack, Decay, Sustain, Release), and volume slider.
- **Mono/Stereo Switch**: Switch between mono and stereo modes.
- **Built-In MIDI Keyboard**: Play your tunes using the built-in MIDI keyboard.

## Key Mapping 🎹

- **C3 to B3**: Adjust pitch from -12 to -1 semitones
- **C4 to B4**: Adjust pitch from +1 to +12 semitones
- **C5 to B5**: Control tempo from 1/2 to 1/128

## Requirements 🛠️

- **Visual Studio 2022**: Ensure you have Visual Studio 2022 installed for building the plugin.
- **Projucer 7.0.8**: Utilize Projucer version 7.0.8 to manage your project files effectively.

## Installation 🚀

1. **Get the Source Code:**
Clone the repository and update submodules to ensure you have the latest source code:
```git submodule update --init --recursive```

2. **Build RubberBand Library:**
Open `vendor\rubberband\otherbuilds\rubberband-library.vcxproj` in `Visual Studio 2022`. Configure the project settings for `Debug` and `x64`, then compile (Ctrl+Shift+B).

3. **Copy RubberBand Library:**
Copy the generated `rubberband-library.lib` from `vendor\rubberband\otherbuilds\x64\Debug\` and paste it as `Library\rubberband.lib` in your project.

4. **Configure and Compile:**
- Launch `Projucer` and open the `HiHatzLab.jucer` project file.
- Click on the Visual Studio logo at the top center.
- Select the `Debug` and `x64` environment, then compile the project (Ctrl+Shift+B).

5. **Copy VST3 Plugin:**
Copy the compiled `HiHatzLab.vst3` from `Builds\VisualStudio2022\x64\Debug\VST3\HiHatzLab.vst3\Contents\x86_64-win` and paste it into the VST3 plugins directory (e.g., `C:\Program Files\Common Files\VST3\`).

6. **Integrate with FL Studio:**
- Launch FL Studio and go to `OPTIONS`.
- Select `Manage plugins` and click `Start scan` in the top left corner to detect the new plugin.
- In the `Channel rack`, click the `+` sign, go to `More plugins...`, and find `HiHatzLab [64bit]`.

## How to Use 🎛️

1. Load your desired sample by dragging and dropping the file.
2. Adjust pitch, time signatures, and other parameters in real-time using MIDI keys or the interface controls.
3. Experiment with different settings to create unique sounds and beats!

## Support & Feedback 💬

For support and feedback, please [submit an issue](https://github.com/kamilhajduk/hihatzlab/issues).

## Credits 🙌

Special thanks to [Florian Mrugalla](https://github.com/Mrugalla) for assistance and support.