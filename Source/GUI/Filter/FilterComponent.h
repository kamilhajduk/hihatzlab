#pragma once

#include <JuceHeader.h>
#include "../Config.h"
#include "../../PluginProcessor.h"
#include "./Attachment/TwoPointSliderAttachment.h"

typedef TwoValueSliderAttachment TwoValueSliderAttachment;

class FilterComponent : public juce::Component, public juce::AudioProcessorValueTreeState::Listener
{
public:
    FilterComponent(HiHatzLabAudioProcessor& p, juce::AudioProcessorValueTreeState& vts);
    ~FilterComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void parameterChanged(const juce::String& parameterID, float newValue) override;

private:
    HiHatzLabAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;

    juce::Slider filterSlider;
    juce::Label  filterLabel, lowFrequencyLabel, highFrequencyLabel, unitLabel;
    std::unique_ptr<TwoValueSliderAttachment> filterAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterComponent)
};

