#include "CentTuneComponent.h"

CentTuneComponent::CentTuneComponent(HiHatzLabAudioProcessor& p, juce::AudioProcessorValueTreeState& vts) : audioProcessor(p), valueTreeState(vts)
{
    // Sliders
    addAndMakeVisible(centSlider);
    centSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    centSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, Config::TEXTBOX_WIDTH, Config::TEXTBOX_HEIGHT);
    centAttachment.reset(new SliderAttachment(valueTreeState, "cent", centSlider));
    centSlider.setRange(-50.0, 50.0, 1.0);

    // Labels
    addAndMakeVisible(centLabel);
    centLabel.setText("Cent", juce::dontSendNotification);
    centLabel.setJustificationType(juce::Justification::centredTop);
    centLabel.attachToComponent(&centSlider, false);

    addAndMakeVisible(lowCentLabel);
    lowCentLabel.setText("-50", juce::dontSendNotification);

    addAndMakeVisible(highCentLabel);
    highCentLabel.setText("50", juce::dontSendNotification);

    addAndMakeVisible(unitLabel);
    unitLabel.setText(" cent", juce::dontSendNotification);

    // Size
    setSize(250, 120);
}

CentTuneComponent::~CentTuneComponent()
{
}

void CentTuneComponent::paint(juce::Graphics& g)
{
    if constexpr (Config::BORDER > 0)
    {
        juce::Rectangle<int> rect(0, 0, getWidth(), getHeight());
        g.setColour(Config::getBorderColor());
        g.drawRect(rect, Config::BORDER);
    }
}

void CentTuneComponent::resized()
{
    centSlider.setBounds(Config::PADDING + Config::LABEL_WIDTH, Config::PADDING + Config::TEXTBOX_HEIGHT, getWidth() - 2*Config::PADDING - 2*Config::LABEL_WIDTH, getHeight() - Config::PADDING - (Config::PADDING + Config::TEXTBOX_HEIGHT));
    lowCentLabel.setBounds(Config::PADDING, 48, Config::LABEL_WIDTH, Config::LABEL_HEIGHT);
    highCentLabel.setBounds(getWidth() - Config::LABEL_WIDTH - Config::PADDING, 48, Config::LABEL_WIDTH, Config::LABEL_HEIGHT);
    unitLabel.setBounds((getWidth() / 2) + 25, getHeight() - (Config::PADDING + Config::LABEL_HEIGHT), 40, Config::LABEL_HEIGHT);
}