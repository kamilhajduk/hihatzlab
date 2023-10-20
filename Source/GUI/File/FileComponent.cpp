#include "FileComponent.h"

FileComponent::FileComponent(HiHatzLabAudioProcessor& p) : audioProcessor(p)
{
    if (audioProcessor.getSampleBuffer().getNumSamples() > 0)
    {
        // We got sample file, set flag to render waveform
        shouldRenderWaveform = true;  
    }
    else
    {
        // File not loaded, display Drag&Drop label
        addAndMakeVisible(dragFileLabel);
        dragFileLabel.setText("Drag file here.", juce::dontSendNotification);
        shouldRenderWaveform = false;
    }

    // Prepare icons
    loadImage = juce::ImageCache::getFromMemory(BinaryData::load_png, BinaryData::load_pngSize);
    importImage = juce::ImageCache::getFromMemory(BinaryData::import_png, BinaryData::import_pngSize);
    exportImage = juce::ImageCache::getFromMemory(BinaryData::export_png, BinaryData::export_pngSize);

    // Labels
    addAndMakeVisible(filenameLabel);
    filenameLabel.setText("Filename: ", juce::dontSendNotification);

    addAndMakeVisible(filenameTextLabel);
    filenameTextLabel.setText(audioProcessor.getFileName(), juce::dontSendNotification);

    // Import button
    addAndMakeVisible(importButton);

    // Use import icon image as button background
    importButton.setImages(false, true, true, 
                           importImage, 0.5f, juce::Colours::white, 
                           importImage, 0.5f, juce::Colours::white,
                           importImage, 0.5f, juce::Colours::white
    );

    // If import button was clicked
    importButton.onClick = [&]() {
        // Prepare flags to open file
        auto folderChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        // Create FileChooser for *.hhl extension
        fileChooser = std::make_unique<juce::FileChooser>("Please select file you want to import...",
            juce::File::getSpecialLocation(juce::File::userHomeDirectory),
            "*.hhl");

        // Launch dialog box for file selection
        fileChooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser)
            {
                // Get selected file
                juce::File file = chooser.getResult();

                // Import APVTS state from preset file
                audioProcessor.importPresetFile(file);

                // Repaint component to redraw sliders, knobs etc.
                repaint();
            });
    };

    // Export button
    addAndMakeVisible(exportButton);

    // Use export icon image as button background
    exportButton.setImages(false, true, true,
        exportImage, 0.5f, juce::Colours::white,
        exportImage, 0.5f, juce::Colours::white,
        exportImage, 0.5f, juce::Colours::white
    );

    // If export button was clicked
    exportButton.onClick = [&]() {
        // Prepare flags for save file
        auto folderChooserFlags = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::warnAboutOverwriting;

        // Create FileChooser for *.hhl extension
        fileChooser = std::make_unique<juce::FileChooser>("Please select file you want to export...",
            juce::File::getSpecialLocation(juce::File::userHomeDirectory),
            "*.hhl");

        // Launch dialog box for file select
        fileChooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser)
            {
                // Get selected file
                juce::File file = chooser.getResult();

                // Export APVTS state into preset file
                audioProcessor.exportPresetFile(file);
            });
    };

    // Load file button
    addAndMakeVisible(loadButton);

    // Use load icon image as button background
    loadButton.setImages(false, true, true,
        loadImage, 0.5f, juce::Colours::white,
        loadImage, 0.5f, juce::Colours::white,
        loadImage, 0.5f, juce::Colours::white
    );

    // If load button was clicked
    loadButton.onClick = [&]() {
        // Prepare flags for open file
        auto folderChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        // Create FileChooser for well-known audio extensions
        fileChooser = std::make_unique<juce::FileChooser>("Please select file you want to load...",
            juce::File::getSpecialLocation(juce::File::userHomeDirectory),
            "*.wav;*.mp3;*.aif");

        // Launch dialog box for file select
        fileChooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser)
        {
            // Get selected file
            juce::File file = chooser.getResult();

            if (file.exists())
            {
                // Load sample buffer from audio file
                audioProcessor.loadFile(file);

                // Disable Drag&Drop label
                dragFileLabel.setVisible(false);

                // We got audio file, set flag to render waveform
                shouldRenderWaveform = true;

                // Display filename inside label
                filenameTextLabel.setText(audioProcessor.getFileName(), juce::dontSendNotification);

                // Repain component to redraw waveform and filename
                repaint();
            }
        });
    };

    // PlayHead Timer
    startTimerHz(100);

    // Size
    setSize(250, 260);
}

FileComponent::~FileComponent()
{
    // Clear pointers
    fileChooser = nullptr;

    // Stop PlayHead timer
    stopTimer();
}

void FileComponent::paint(juce::Graphics& g)
{
    // Draw border
    if constexpr (Config::BORDER > 0)
    {
        juce::Rectangle<int> rectTop(0, 0, getWidth(), 200);
        juce::Rectangle<int> rectBottom(0, 200 + Config::MARGIN, getWidth(), 40);
        g.setColour(Config::getBorderColor());
        g.drawRect(rectTop, Config::BORDER);
        g.drawRect(rectBottom, Config::BORDER);
    }
    

    // Waveform and playhead should be rendered
    if (audioProcessor.getSampleBuffer().getNumSamples() > 0)
    {
        // Get buffer
        juce::AudioSampleBuffer sampleBuffer = audioProcessor.getSampleBuffer();

        // Clear previous waveform
        path.clear();
        audioPoints.clear();

        // Calculate ratio
        ratio = sampleBuffer.getNumSamples() / (getWidth() - 2*Config::PADDING);

        // Save samples scaled by ratio
        const float* samplePoints = sampleBuffer.getReadPointer(0);
        for (int i = 0; i < sampleBuffer.getNumSamples(); i += ratio)
        {
            audioPoints.push_back(samplePoints[i]);
        }

        // Set starting point for waveform
        path.startNewSubPath(Config::PADDING, 100);

        // For every scaled sample
        for (int i = 0; i < audioPoints.size(); ++i)
        {
            // Scale float value between -1.0 and 1.0 to pixel value between 0px and 100px
            auto point = juce::jmap<float>(audioPoints[i], -1.0f, 1.0f, 200.0f - Config::PADDING, Config::PADDING);

            // Draw next point in waveform
            path.lineTo(static_cast<float>(i+Config::PADDING), point);
        }

        // Set waveform color
        g.setColour(Config::getWaveformColor());

        // Draw waveform stroke
        g.strokePath(path, juce::PathStrokeType(1.0f));

        // Draw Playhead
        int playheadPosition = juce::jmap<int>(audioProcessor.getSampleIndex(), 0, audioProcessor.getSampleBuffer().getNumSamples(), 0, getWidth());
        g.setColour(Config::getPlayheadColor());
        g.drawLine(static_cast<float>(playheadPosition + Config::PADDING), Config::PADDING, static_cast<float>(playheadPosition + Config::PADDING), 200.0f - Config::PADDING, 0.5f);
    }
}

void FileComponent::resized()
{
    dragFileLabel.setBounds(75, 90, 100, Config::LABEL_HEIGHT);
    filenameLabel.setBounds(Config::PADDING, getHeight() - Config::PADDING - Config::LABEL_HEIGHT, 55, Config::LABEL_HEIGHT);
    filenameTextLabel.setBounds(60, getHeight() - Config::PADDING - Config::LABEL_HEIGHT, 110, Config::LABEL_HEIGHT);

    importButton.setBounds(170, getHeight() - Config::PADDING - Config::LABEL_HEIGHT, Config::ICON_SIZE, Config::ICON_SIZE);
    exportButton.setBounds(195, getHeight() - Config::PADDING - Config::LABEL_HEIGHT, Config::ICON_SIZE, Config::ICON_SIZE);
    loadButton.setBounds(  220, getHeight() - Config::PADDING - Config::LABEL_HEIGHT, Config::ICON_SIZE, Config::ICON_SIZE);
}

bool FileComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    // Validation if Drag&Drop filename extension is good
    for (juce::String file : files)
    {
        if (file.contains(".wav") || file.contains(".mp3") || file.contains(".aif"))
        {
            return true;
        }
    }
    return false;
}

void FileComponent::filesDropped(const juce::StringArray& files, int x, int y)
{
    juce::ignoreUnused(x);
    juce::ignoreUnused(y);

    // For every file that was Drag&Drop
    for (juce::String filename : files)
    {
        // Validate extension
        if (isInterestedInFileDrag(filename))
        {
            // Prepare file object
            juce::File file = juce::File(filename);

            // Disable Drag&Drop label
            dragFileLabel.setVisible(false);

            // Set flag that we should render waveform
            shouldRenderWaveform = true;

            // Load file into buffers needed in audio processor
            audioProcessor.loadFile(file);

            // Display filename inside label
            filenameTextLabel.setText(audioProcessor.getFileName(), juce::dontSendNotification);

            // Repaint whole component to draw waveform
            repaint();
        }
    }
}

void FileComponent::timerCallback()
{
    repaint();
}