#pragma once

#include <JuceHeader.h>
#include "../Config.h"
#include "../../PluginProcessor.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class VolumeComponent : public juce::Component
{
public:
    VolumeComponent(HiHatzLabAudioProcessor& p, juce::AudioProcessorValueTreeState& vts);
    ~VolumeComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    HiHatzLabAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;

    juce::Slider volumeSlider;
    juce::Label  volumeLabel;
    std::unique_ptr<SliderAttachment> volumeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VolumeComponent)
};
