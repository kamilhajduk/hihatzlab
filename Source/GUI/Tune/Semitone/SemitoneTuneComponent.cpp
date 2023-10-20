#include "SemitoneTuneComponent.h"

SemitoneTuneComponent::SemitoneTuneComponent(HiHatzLabAudioProcessor& p, juce::AudioProcessorValueTreeState& vts) : audioProcessor(p), valueTreeState(vts)
{
    // Sliders
    addAndMakeVisible(semitoneSlider);
    semitoneSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    semitoneSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, Config::TEXTBOX_WIDTH, Config::TEXTBOX_HEIGHT);
    semitoneAttachment.reset(new SliderAttachment(valueTreeState, "semitone", semitoneSlider));
    semitoneSlider.setRange(-24.0, 24.0, 1.0);

    // Labels
    addAndMakeVisible(semitoneLabel);
    semitoneLabel.setText("Semitone", juce::dontSendNotification);
    semitoneLabel.setJustificationType(juce::Justification::centredTop);
    semitoneLabel.attachToComponent(&semitoneSlider, false);

    addAndMakeVisible(lowSemitoneLabel);
    lowSemitoneLabel.setText("-24", juce::dontSendNotification);

    addAndMakeVisible(highSemitoneLabel);
    highSemitoneLabel.setText("24", juce::dontSendNotification);

    addAndMakeVisible(unitLabel);
    unitLabel.setText(" semi", juce::dontSendNotification);

    // Size
    setSize(250, 120);
}

SemitoneTuneComponent::~SemitoneTuneComponent()
{
}

void SemitoneTuneComponent::paint(juce::Graphics& g)
{
    if constexpr (Config::BORDER > 0)
    {
        juce::Rectangle<int> rect(0, 0, getWidth(), getHeight());
        g.setColour(Config::getBorderColor());
        g.drawRect(rect, Config::BORDER);
    }
}

void SemitoneTuneComponent::resized()
{
    semitoneSlider.setBounds(Config::PADDING + Config::LABEL_WIDTH, Config::PADDING + Config::TEXTBOX_HEIGHT, getWidth() - 2 * Config::PADDING - 2 * Config::LABEL_WIDTH, getHeight() - Config::PADDING - (Config::PADDING + Config::TEXTBOX_HEIGHT));
    lowSemitoneLabel.setBounds(Config::PADDING, 48, Config::LABEL_WIDTH, Config::LABEL_HEIGHT);
    highSemitoneLabel.setBounds(getWidth() - Config::LABEL_WIDTH - Config::PADDING, 48, Config::LABEL_WIDTH, Config::LABEL_HEIGHT);
    unitLabel.setBounds((getWidth() / 2) + 25, getHeight() - (Config::PADDING + Config::LABEL_HEIGHT), 40, Config::LABEL_HEIGHT);
}