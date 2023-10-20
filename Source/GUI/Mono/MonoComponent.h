#pragma once

#include <JuceHeader.h>
#include "../Config.h"
#include "../../PluginProcessor.h"

typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

class MonoComponent : public juce::Component
{
public:
    MonoComponent(HiHatzLabAudioProcessor& p, juce::AudioProcessorValueTreeState& vts);
    ~MonoComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    HiHatzLabAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;

    juce::ToggleButton monoButton;
    std::unique_ptr<ButtonAttachment> monoAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MonoComponent)
};
