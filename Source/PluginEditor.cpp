#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HiHatzLabAudioProcessorEditor::HiHatzLabAudioProcessorEditor(HiHatzLabAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p),
    audioProcessor(p),
    valueTreeState(vts),
    fileComponent(p),
    semitoneTuneComponent(p, vts),
    centTuneComponent(p, vts),
    adsrComponent(p, vts),
    filterComponent(p, vts),
    volumeComponent(p, vts),
    monoComponent(p, vts),
    bpmComponent(p, vts),
    keyboardComponent(audioProcessor.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible(fileComponent);
    addAndMakeVisible(semitoneTuneComponent);
    addAndMakeVisible(centTuneComponent);
    addAndMakeVisible(filterComponent);
    addAndMakeVisible(adsrComponent);
    addAndMakeVisible(volumeComponent);
    addAndMakeVisible(monoComponent);
    addAndMakeVisible(bpmComponent);
    addAndMakeVisible(keyboardComponent);

    // Handle MIDI notes from on-screen MIDI keyboard (handleNoteOn/handleNoteOff)
    audioProcessor.keyboardState.addListener(this);

    // MIDI notes range from 36 to 71 (C3 - B5)
    keyboardComponent.setAvailableRange(36, 71);

    // Key width = MIDI keyboard width / 21
    keyboardComponent.setKeyWidth(44.285f);

    // Middle C is C5
    keyboardComponent.setOctaveForMiddleC(5);

    // Keypress base octave is 5
    keyboardComponent.setKeyPressBaseOctave(5);

    // Grab keyboard focus by MIDI keyboard
    keyboardComponent.grabKeyboardFocus();

    // Size
    setSize (930, 480);
}

HiHatzLabAudioProcessorEditor::~HiHatzLabAudioProcessorEditor()
{
    audioProcessor.keyboardState.removeListener(this);
}

//==============================================================================
void HiHatzLabAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void HiHatzLabAudioProcessorEditor::resized()
{
    fileComponent.setBounds(20, 20, 250, 260);
    semitoneTuneComponent.setBounds(290, 20, 250, 120);
    centTuneComponent.setBounds(290, 160, 250, 120);
    filterComponent.setBounds(560, 20, 250, 120);
    adsrComponent.setBounds(560, 160, 250, 120);
    volumeComponent.setBounds(830, 20, 80, 120);
    bpmComponent.setBounds(830, 160, 80, 70);
    monoComponent.setBounds(830, 240, 80, 40);
    keyboardComponent.setBounds(0, 300, getWidth(), 180);
}

void HiHatzLabAudioProcessorEditor::handleNoteOn(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    juce::ignoreUnused(source);
    juce::ignoreUnused(midiChannel);
    juce::ignoreUnused(velocity);

    // Handle on-screen MIDI keyboard "Note On" event by audio processor
    audioProcessor.handleNoteOn(midiNoteNumber);
}

void HiHatzLabAudioProcessorEditor::handleNoteOff(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    juce::ignoreUnused(source);
    juce::ignoreUnused(midiChannel);
    juce::ignoreUnused(velocity);

    // Handle on-screen MIDI keyboard "Note Off" event by audio processor
    audioProcessor.handleNoteOff(midiNoteNumber);
}