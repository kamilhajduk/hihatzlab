#pragma once

#include <JuceHeader.h>
#include "../../Config.h"
#include "../../../PluginProcessor.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class CentTuneComponent : public juce::Component
{
public:
    CentTuneComponent(HiHatzLabAudioProcessor& p, juce::AudioProcessorValueTreeState& vts);
    ~CentTuneComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    HiHatzLabAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;

    juce::Slider centSlider;
    juce::Label  centLabel, lowCentLabel, highCentLabel, unitLabel;
    std::unique_ptr<SliderAttachment> centAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CentTuneComponent)
};
