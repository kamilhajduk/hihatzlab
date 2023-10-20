#pragma once

#include <JuceHeader.h>
#include "../../Config.h"
#include "../../../PluginProcessor.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class SemitoneTuneComponent : public juce::Component
{
public:
    SemitoneTuneComponent(HiHatzLabAudioProcessor& p, juce::AudioProcessorValueTreeState& vts);
    ~SemitoneTuneComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    HiHatzLabAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;

    juce::Slider semitoneSlider;
    juce::Label  semitoneLabel, lowSemitoneLabel, highSemitoneLabel, unitLabel;
    std::unique_ptr<SliderAttachment> semitoneAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SemitoneTuneComponent)
};
