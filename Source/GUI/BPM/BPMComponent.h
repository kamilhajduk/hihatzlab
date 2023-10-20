#pragma once

#include <JuceHeader.h>
#include "../Config.h"
#include "../../PluginProcessor.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class BPMComponent : public juce::Component
{
public:
    BPMComponent(HiHatzLabAudioProcessor& p, juce::AudioProcessorValueTreeState& vts);
    ~BPMComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    HiHatzLabAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;

    juce::Slider bpmSlider;
    juce::Label  bpmLabel;
    std::unique_ptr<SliderAttachment> bpmAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BPMComponent)
};
