#include "PluginProcessor.h"
#include "PluginEditor.h"

HiHatzLabAudioProcessor::HiHatzLabAudioProcessor() : AudioProcessor (BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
adsrParameters(0.0, 0.0, 1.0, 0.0),
parameters(*this, nullptr, juce::Identifier("HiHatzLabParameters"),
{
    std::make_unique<juce::AudioParameterInt>("semitone", "Semitone", -24, 24, 0),
    std::make_unique<juce::AudioParameterInt>("cent", "Cent", -50, 50, 0),
    std::make_unique<juce::AudioParameterInt>("lowFrequency", "Low Frequency", 20, 20480, 20),
    std::make_unique<juce::AudioParameterInt>("highFrequency", "High Frequency", 20, 20480, 20480),
    std::make_unique<juce::AudioParameterFloat>("attack", "Attack", 0.0f, 5.0f, 0.0f),
    std::make_unique<juce::AudioParameterFloat>("decay", "Decay", 0.0f, 5.0f, 0.0f),
    std::make_unique<juce::AudioParameterFloat>("sustain", "Sustain", 0.0f, 1.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>("release", "Release", 0.0f, 5.0f, 0.0f),
    std::make_unique<juce::AudioParameterInt>("volume", "Volume", 0, 100, 100),
    std::make_unique<juce::AudioParameterFloat>("bpm", "BPM", 10.0f, 522.0f, 130.0f),
    std::make_unique<juce::AudioParameterBool>("mono", "Mono", false)
})
{
    // Register basic formats, like WAV, MP3 etc.
    formatManager.registerBasicFormats();

    // Save APVTS parameters value as pointer
    semitone = parameters.getRawParameterValue("semitone");
    cent = parameters.getRawParameterValue("cent");
    lowFrequency = parameters.getRawParameterValue("lowFrequency");
    highFrequency = parameters.getRawParameterValue("highFrequency");
    attack = parameters.getRawParameterValue("attack");
    decay = parameters.getRawParameterValue("decay");
    sustain = parameters.getRawParameterValue("sustain");
    release = parameters.getRawParameterValue("release");
    volume = parameters.getRawParameterValue("volume");
    bpm = parameters.getRawParameterValue("bpm");
    mono = parameters.getRawParameterValue("mono");

    // Prepare Tempo MIDI map
    dividerMap[60] = 2;
    dividerMap[61] = 3;
    dividerMap[62] = 4;
    dividerMap[63] = 6;
    dividerMap[64] = 8;
    dividerMap[65] = 16;
    dividerMap[66] = 12;
    dividerMap[67] = 32;
    dividerMap[68] = 24;
    dividerMap[69] = 64;
    dividerMap[70] = 48;
    dividerMap[71] = 128;

    // Prepare Pitch Down MIDI map
    pitchMap[36] = -12;
    pitchMap[37] = -11;
    pitchMap[38] = -10;
    pitchMap[39] = -9;
    pitchMap[40] = -8;
    pitchMap[41] = -7;
    pitchMap[42] = -6;
    pitchMap[43] = -5;
    pitchMap[44] = -4;
    pitchMap[45] = -3;
    pitchMap[46] = -2;
    pitchMap[47] = -1;

    // Prepare Pitch Up MIDI map
    pitchMap[48] = 1;
    pitchMap[49] = 2;
    pitchMap[50] = 3;
    pitchMap[51] = 4;
    pitchMap[52] = 5;
    pitchMap[53] = 6;
    pitchMap[54] = 7;
    pitchMap[55] = 8;
    pitchMap[56] = 9;
    pitchMap[57] = 10;
    pitchMap[58] = 11;
    pitchMap[59] = 12;
}

HiHatzLabAudioProcessor::~HiHatzLabAudioProcessor()
{
    // Clear pointers
    playHead = nullptr;
    stretcher = nullptr;
    formatReader = nullptr;
}
const juce::String HiHatzLabAudioProcessor::getName() const { return JucePlugin_Name; }
bool HiHatzLabAudioProcessor::acceptsMidi() const { return true; }
bool HiHatzLabAudioProcessor::producesMidi() const { return false; }
bool HiHatzLabAudioProcessor::isMidiEffect() const { return false; }
double HiHatzLabAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int HiHatzLabAudioProcessor::getNumPrograms() { return 1; }
int HiHatzLabAudioProcessor::getCurrentProgram() { return 0; }
void HiHatzLabAudioProcessor::setCurrentProgram (int index) {
    juce::ignoreUnused(index);
}
const juce::String HiHatzLabAudioProcessor::getProgramName (int index) {
    juce::ignoreUnused(index);
    return {};
}
void HiHatzLabAudioProcessor::changeProgramName (int index, const juce::String& newName) {
    juce::ignoreUnused(index);
    juce::ignoreUnused(newName);
}
void HiHatzLabAudioProcessor::releaseResources() {}
bool HiHatzLabAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* HiHatzLabAudioProcessor::createEditor() { return new HiHatzLabAudioProcessorEditor(*this, parameters); }
void HiHatzLabAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
    juce::ignoreUnused(destData);
}
void HiHatzLabAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    juce::ignoreUnused(data);
    juce::ignoreUnused(sizeInBytes);
}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new HiHatzLabAudioProcessor(); }
#ifndef JucePlugin_PreferredChannelConfigurations
bool HiHatzLabAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
    juce::ignoreUnused(layouts);
    return true;
}
#endif

//==============================================================================
void HiHatzLabAudioProcessor::prepareToPlay(double newSampleRate, int samplesPerBlock)
{
    // Store sample rate for later use
    sampleRate = newSampleRate;

    // Reset sample timing
    resetSample();

    // Set all note state to "Note Off"
    std::fill(notesState, notesState + 128, false);

    // Init RubberBand library (Pitch Shift)
    stretcher = new RubberBand::RubberBandStretcher(static_cast<size_t>(sampleRate), 2, RubberBand::RubberBandStretcher::OptionProcessRealTime);
    stretcher->setMaxProcessSize(samplesPerBlock);

    // Prepare ADSR envelope
    adsr.setSampleRate(sampleRate);
    adsr.setParameters(adsrParameters);
}

void HiHatzLabAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    // AudioBuffer info
    int bufferChannels = buffer.getNumChannels();
    const int bufferLength = buffer.getNumSamples();

    // Sample buffer info
    const int sampleChannels = sampleBuffer.getNumChannels();
    const int sampleLength = sampleBuffer.getNumSamples();

    // Get current BPM value
    playheadBpm = getBpm();
    if (playheadBpm > 0 && playheadBpm != static_cast<double>(*bpm))
    {
        // Get BPM parameter pointer
        juce::RangedAudioParameter* bpmParameter = parameters.getParameter("bpm");

        // Set BPM value from PlayHead
        bpmParameter->setValueNotifyingHost(bpmParameter->convertTo0to1(static_cast<float>(playheadBpm)));
    }

    // Handle MIDI messages
    for (const auto metadata : midiMessages)
    {
        const juce::MidiMessage message = metadata.getMessage();

        if (message.isNoteOn())
        {
            // Get MIDI note number (0 to 127)
            int noteNumber = message.getNoteNumber();

            // Handle "Note On" event
            handleNoteOn(noteNumber);

            // Process "Note On" event by MidiKeyboardState.
            // On-screen MIDI Keyboard will be triggered
            keyboardState.processNextMidiEvent(message);

        }
        else if (message.isNoteOff())
        {
            // Get MIDI note number (0 to 127)
            int noteNumber = message.getNoteNumber();

            // Handle "Note Off" event
            handleNoteOff(noteNumber);

            // Process "Note Off" event by MidiKeyboardState.
            // On-screen MIDI Keyboard will be triggered
            keyboardState.processNextMidiEvent(message);
        }
    }

    // Default flag - don't play sample
    shouldPlay = false;

    // Default time signature
    divider = 1;

    // Default pitch shift factor
    pitchFactor = 0;

    // Check if any tempo note was pressed and not released
    for (int noteIndex = 60; noteIndex <= 71; noteIndex++)
    {
        // Tempo note is pressed
        if (notesState[noteIndex] == true)
        {
            // We should play audio sample
            shouldPlay = true;

            // Tempo divider: fastest note should be picked
            divider = dividerMap[noteIndex];
        }
    }

    // Check if any tempo note was pressed and not released
    for (int noteIndex = 36; noteIndex <= 59; noteIndex++)
    {
        // Pitch note is pressed
        if (notesState[noteIndex] == true)
        {
            // Pitch Shift Factor: highest pitch note should be picked
            pitchFactor = pitchMap[noteIndex];
        }
    }

    // Calculate pitch shift factor using current pitch factor, semitone and cent value
    double pitchRatio = pitchFactor;
    pitchRatio += *semitone / 12;
    pitchRatio += *cent / 1200;

    // Set RubberBand pitch shift scale
    stretcher->setPitchScale(std::pow(2.0, pitchRatio / 12));

    // Calculate duration (samplesPerBeat / tempoDivider)
    sampleDuration = (sampleRate * 60.0 / *bpm) / divider;

    // For every floating point in buffer
    for (int j = 0; j < bufferLength; ++j)
    {
        // Timer tick = time to reset sample
        if (sampleTimer >= sampleDuration)
        {
            // Reset sample timing and position
            resetSample();
        }

        // If audio still need to be played
        if (shouldPlay && sampleIndex < sampleLength)
        {
            // For every audio buffer channel
            for (int channel = 0; channel < sampleChannels; ++channel)
            {
                // Get single floating point from sample buffer by position
                float singleSample = sampleBuffer.getSample(channel, sampleIndex);

                // Calculate volume percentage
                double volumePercentage = (*volume / 100);

                // Set audio buffer sample (multiplied by volume)
                buffer.setSample(channel, j, static_cast<float>(singleSample * volumePercentage));
            }

            // Increment sample position by one
            sampleIndex++;
        }
        else
        {
            // Audio shouldn't be played
            for (int channel = 0; channel < bufferChannels; ++channel)
            {
                // For every audio buffer channel - clear sample
                buffer.setSample(channel, j, 0);
            }
        }

        // Increase sample timer by one
        sampleTimer++;
    }

    // If audio sample should be playing
    if (shouldPlay)
    {
        // Pitch shift, if needed
        if (pitchRatio != 0.0)
        {
            // Load whole audio block to process by RubberBand
            stretcher->process(buffer.getArrayOfReadPointers(), bufferLength, sampleIndex >= sampleLength);

            // Write RubberBand output into output audio buffer
            stretcher->retrieve(buffer.getArrayOfWritePointers(), bufferLength);
        }

        // Band-pass filter
        for (int channel = 0; channel < bufferChannels; ++channel)
        {
            // For every audio buffer channel - apply low and high pass filter
            highPassfilter.processSamples(buffer.getWritePointer(channel), bufferLength);
            lowPassfilter.processSamples(buffer.getWritePointer(channel), bufferLength);
        }

        // Apply ADSR envelope
        adsr.applyEnvelopeToBuffer(buffer, 0, bufferLength);

        // Mono?
        if (*mono)
        {
            // If mono selected, clear every channel except channel 0
            for (int channel = 1; channel < bufferChannels; ++channel)
            {
                buffer.clear(channel, 0, bufferLength);
            }
        }
    }
}

void HiHatzLabAudioProcessor::loadFile(juce::File file)
{
    // Check if file exists
    if (!file.exists())
        return;

    // Save audio file
    sampleFile = file;

    // Prepare format reader for this file
    formatReader = formatManager.createReaderFor(sampleFile);

    if (formatReader != nullptr)
    {
        // File loaded successfully, prepare audio file buffer
        sampleBuffer.setSize((int)formatReader->numChannels, (int)formatReader->lengthInSamples);

        // Read audio file content into sample buffer
        formatReader->read(&sampleBuffer, 0, (int)formatReader->lengthInSamples, 0, true, true);

        // Save filename
        filename = sampleFile.getFileName();
    }
}

double HiHatzLabAudioProcessor::getBpm()
{
    // Get PlayHead pointer
    playHead = getPlayHead();

    if (playHead != nullptr)
    {
        // Get position info
         juce::Optional<juce::AudioPlayHead::PositionInfo> positionInfo = playHead->getPosition();

         if (positionInfo.hasValue())
         {
             // Get current BPM from position info
             juce::Optional<double> currentBpm = positionInfo->getBpm();
             
             if (currentBpm.hasValue())
             {
                 // Check if the host is providing correct BPM value
                 if (*currentBpm > 0)
                 {
                     // Return BPM value
                     return *currentBpm;
                 }
             }
         }
    }

    return 0;
}

void HiHatzLabAudioProcessor::resetSample()
{
    // Reset sample position
    sampleIndex = 0;

    // Reset sample timer
    sampleTimer = 0;

    // Reset ADSR envelope
    adsr.reset();
    adsr.noteOn();
}

void HiHatzLabAudioProcessor::handleNoteOn(int midiNoteNumber)
{
    // Set note state flag - Note On
    notesState[midiNoteNumber] = true;

    // If note is tempo note
    if (midiNoteNumber >= 60 && midiNoteNumber <= 71)
    {
        // Reset sample
        resetSample();
    }
}

void HiHatzLabAudioProcessor::handleNoteOff(int midiNoteNumber)
{
    // Set note state flag - Note Off
    notesState[midiNoteNumber] = false;
}

void HiHatzLabAudioProcessor::importPresetFile(const juce::File& presetFile)
{
    // File doesn't exist
    if (!presetFile.existsAsFile())
        return; 

    // Prepare input stream
    juce::FileInputStream inputStream(presetFile);

    // Failed to open file
    if (!inputStream.openedOk())
        return; 

    // Read file content as one string
    juce::String fileContent = inputStream.readEntireStreamAsString();

    // Parse content to XML object
    std::unique_ptr<juce::XmlElement> xmlState = juce::XmlDocument::parse(fileContent);
    if (xmlState != nullptr)
    {
        // Read parameters from XML object
        juce::ValueTree state = juce::ValueTree::fromXml(*xmlState);

        // Apply parameters to APVTS
        parameters.replaceState(state);
    }
}

void HiHatzLabAudioProcessor::exportPresetFile(const juce::File& presetFile)
{
    // Get current APVTS state
    juce::ValueTree state = parameters.copyState();

    // Convert state to XML object
    std::unique_ptr<juce::XmlElement> xml(state.createXml());

    // Save it to file
    xml->writeTo(presetFile);
}