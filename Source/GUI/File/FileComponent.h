#pragma once

#include <JuceHeader.h>
#include "../Config.h"
#include "../../PluginProcessor.h"

class FileComponent : public juce::Component, public juce::FileDragAndDropTarget, public juce::Timer
{
public:
    FileComponent(HiHatzLabAudioProcessor& p);
    ~FileComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    void timerCallback() override;

private:
    HiHatzLabAudioProcessor& audioProcessor;
    std::unique_ptr<juce::FileChooser> fileChooser;

    // Buttons
    juce::ImageButton loadButton, importButton, exportButton;
    juce::Image loadImage, importImage, exportImage;
    
    // Waveform
    bool shouldRenderWaveform = false;
    int ratio = 0;
    std::vector<float> audioPoints;
    juce::Path path;

    // Labels
    juce::Label  dragFileLabel, filenameLabel, filenameTextLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileComponent)
};
