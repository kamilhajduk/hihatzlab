#include "FilterComponent.h"

FilterComponent::FilterComponent(HiHatzLabAudioProcessor& p, juce::AudioProcessorValueTreeState& vts) : audioProcessor(p), valueTreeState(vts)
{
    // APVTS parameters change listeners for frequencies
    valueTreeState.addParameterListener("lowFrequency", this);
    valueTreeState.addParameterListener("highFrequency", this);

    // Sliders
    addAndMakeVisible(filterSlider);
    filterSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, Config::TEXTBOX_HEIGHT);
    filterSlider.setSliderStyle(juce::Slider::SliderStyle::TwoValueHorizontal);
    filterAttachment.reset(new TwoValueSliderAttachment(valueTreeState, "lowFrequency", "highFrequency", filterSlider));
    filterSlider.setRange(20.0, 20480.0, 1.0);

    // Set frequencies according to APVTS
    filterSlider.setMinAndMaxValues(
        static_cast<double>(valueTreeState.getParameterAsValue("lowFrequency").getValue()),
        static_cast<double>(valueTreeState.getParameterAsValue("highFrequency").getValue()),
        juce::sendNotification
    );

    // Log2 scale from 20 to 20480 (middle point is 640)
    filterSlider.setSkewFactorFromMidPoint(640.0);

    // Labels
    addAndMakeVisible(filterLabel);
    filterLabel.setText("Filter", juce::dontSendNotification);
    filterLabel.setJustificationType(juce::Justification::centredTop);
    filterLabel.attachToComponent(&filterSlider, false);

    addAndMakeVisible(lowFrequencyLabel);
    lowFrequencyLabel.setText("20", juce::dontSendNotification);

    addAndMakeVisible(highFrequencyLabel);
    highFrequencyLabel.setText("20k", juce::dontSendNotification);

    addAndMakeVisible(unitLabel);
    unitLabel.setText(" Hz", juce::dontSendNotification);
    
    // Size
    setSize(250, 120);
}

FilterComponent::~FilterComponent()
{
    // Remove APVTS listeners for frequency parameter change
    valueTreeState.removeParameterListener("lowFrequency", this);
    valueTreeState.removeParameterListener("highFrequency", this);
}

void FilterComponent::paint(juce::Graphics& g)
{
    if constexpr (Config::BORDER > 0)
    {
        juce::Rectangle<int> rect(0, 0, getWidth(), getHeight());
        g.setColour(Config::getBorderColor());
        g.drawRect(rect, Config::BORDER);
    }
}

void FilterComponent::resized()
{
    filterSlider.setBounds(Config::PADDING + Config::LABEL_WIDTH, Config::PADDING + Config::TEXTBOX_HEIGHT, getWidth() - 2*Config::PADDING - 2*Config::LABEL_WIDTH, getHeight() - Config::PADDING - (Config::PADDING + Config::TEXTBOX_HEIGHT));
    lowFrequencyLabel.setBounds(Config::PADDING, Config::PADDING + 2* Config::TEXTBOX_HEIGHT, Config::LABEL_WIDTH, Config::LABEL_HEIGHT);
    highFrequencyLabel.setBounds(getWidth() - Config::LABEL_WIDTH - Config::PADDING, Config::PADDING + 2 * Config::TEXTBOX_HEIGHT, Config::LABEL_WIDTH, Config::LABEL_HEIGHT);
    unitLabel.setBounds((getWidth() / 2) + 50, getHeight() - (Config::PADDING + Config::TEXTBOX_HEIGHT), 40, Config::LABEL_HEIGHT);
}

void FilterComponent::parameterChanged(const juce::String& parameterID, float newValue)
{
    // If APVTS parameter changed, edit band-pass filter frequencies in audio processor
    if (parameterID == "lowFrequency")
        audioProcessor.setLowFrequency(newValue);
    else if (parameterID == "highFrequency")
        audioProcessor.setHighFrequency(newValue);
}