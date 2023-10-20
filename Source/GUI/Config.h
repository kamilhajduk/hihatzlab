#pragma once

#include <JuceHeader.h>

class Config
{
public:
    static const int MARGIN = 20;
    static const int PADDING = 10;

    static const int BORDER = 1;

    static juce::Colour getBorderColor() { return juce::Colour(0xff081213); }
    static juce::Colour getWaveformColor() { return juce::Colour(0xff787f84); }
    static juce::Colour getPlayheadColor() { return juce::Colour(0xffffb870); }

    static const int KNOB_WIDTH = 50;

    static const int LABEL_WIDTH = 30;
    static const int LABEL_HEIGHT = 20;

    static const int TEXTBOX_WIDTH = 50;
    static const int TEXTBOX_HEIGHT = 20;

    static const int ICON_SIZE = 20;
};
