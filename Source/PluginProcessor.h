#pragma once

#include <JuceHeader.h>
#include "rubberband/RubberBandStretcher.h"

class HiHatzLabAudioProcessor  : public juce::AudioProcessor
{
public:

    HiHatzLabAudioProcessor();
    ~HiHatzLabAudioProcessor() override;
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================

    /* File */
    void loadFile(juce::File file);
    void importPresetFile(const juce::File& presetFile);
    void exportPresetFile(const juce::File& presetFile);
    juce::AudioSampleBuffer getSampleBuffer() { return sampleBuffer; }
    juce::String getFileName() { return filename;  }

    /* Pitch shift */
    int getSampleIndex() { return sampleIndex; }

    /* Band-pass filter */
    void setLowFrequency(float newLowFrequency) {
        highPassfilter.setCoefficients(juce::IIRCoefficients::makeHighPass(sampleRate, newLowFrequency));
    }
    void setHighFrequency(float newHighFrequency) {
        lowPassfilter.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, newHighFrequency));
    }

    /* ADSR */
    juce::ADSR& getAdsr() { return adsr; }
    juce::ADSR::Parameters getAdsrParameters() { return adsrParameters; }
    void setAdsrParameters(juce::ADSR::Parameters newAdsrParameters) {
        adsrParameters = newAdsrParameters;
        adsr.setParameters(newAdsrParameters);
    }

    /* MIDI keyboard */
    juce::MidiKeyboardState keyboardState;
    void handleNoteOn(int midiNoteNumber);
    void handleNoteOff(int midiNoteNumber);

    /* APVTS */
    juce::AudioProcessorValueTreeState& getParameters() { return parameters; }

private:
    // APVTS
    juce::AudioProcessorValueTreeState parameters;

    std::atomic<float>* semitone = nullptr;
    std::atomic<float>* cent = nullptr;
    std::atomic<float>* lowFrequency = nullptr;
    std::atomic<float>* highFrequency = nullptr;
    std::atomic<float>* attack = nullptr;
    std::atomic<float>* decay = nullptr;
    std::atomic<float>* sustain = nullptr;
    std::atomic<float>* release = nullptr;
    std::atomic<float>* volume = nullptr;
    std::atomic<float>* bpm = nullptr;
    std::atomic<float>* mono = nullptr;

    double sampleRate = 44100.0;

    /* BPM */
    juce::AudioPlayHead* playHead { nullptr };
    double playheadBpm = 130.0;
    double getBpm();

    /* Pitch shift */
    RubberBand::RubberBandStretcher* stretcher { nullptr };
    int pitchFactor = 0;
    int pitchMap[128] = { 0 };

    /* Time Signature */
    bool shouldPlay = false;
    int sampleTimer = 0;
    int sampleIndex = 0;
    double sampleDuration;
    int divider = 1;
    int dividerMap[128] = { 0 };
    void resetSample();

    /* Band-pass filter */
    juce::IIRFilter lowPassfilter;
    juce::IIRFilter highPassfilter;

    /* WAV file */
    juce::AudioFormatManager formatManager;
    juce::AudioFormatReader* formatReader { nullptr };
    juce::File sampleFile;
    juce::AudioSampleBuffer sampleBuffer;
    juce::String filename = "-";

    /* MIDI notes state */
    bool notesState[128] = { false };

    /* ADSR */
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParameters;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HiHatzLabAudioProcessor)
};