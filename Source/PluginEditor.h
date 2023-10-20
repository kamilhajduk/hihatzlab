#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/File/FileComponent.h"
#include "GUI/ADSR/ADSRComponent.h"
#include "GUI/Filter/FilterComponent.h"
#include "GUI/Volume/VolumeComponent.h"
#include "GUI/Mono/MonoComponent.h"
#include "GUI/BPM/BPMComponent.h"
#include "GUI/Tune/Semitone/SemitoneTuneComponent.h"
#include "GUI/Tune/Cent/CentTuneComponent.h"

class HiHatzLabAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::MidiKeyboardStateListener
{
public:
    HiHatzLabAudioProcessorEditor(HiHatzLabAudioProcessor& p, juce::AudioProcessorValueTreeState& vts);
    ~HiHatzLabAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void handleNoteOn(juce::MidiKeyboardState* source, int MidiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(juce::MidiKeyboardState* source, int MidiChannel, int midiNoteNumber, float velocity) override;

private:
    HiHatzLabAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;

    // Components
    FileComponent fileComponent;
    SemitoneTuneComponent semitoneTuneComponent;
    CentTuneComponent centTuneComponent;
    FilterComponent filterComponent;
    ADSRComponent adsrComponent;
    VolumeComponent volumeComponent;
    MonoComponent monoComponent;
    BPMComponent bpmComponent;
    juce::MidiKeyboardComponent keyboardComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HiHatzLabAudioProcessorEditor)
};
