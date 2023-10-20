#pragma once

#include <JuceHeader.h>

class TwoValueSliderAttachment {
public:

    TwoValueSliderAttachment(juce::AudioProcessorValueTreeState& vts, const juce::String minParamID, const juce::String maxParamID, juce::Slider& slider);

    juce::ParameterAttachment minValue;
    juce::ParameterAttachment maxValue;

private:

    juce::Slider& twoPointSlider;
    int lastDraggedThumb = 0;

};