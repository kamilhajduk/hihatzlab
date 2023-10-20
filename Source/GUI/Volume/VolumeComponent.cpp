#include "VolumeComponent.h"

VolumeComponent::VolumeComponent(HiHatzLabAudioProcessor& p, juce::AudioProcessorValueTreeState& vts) : audioProcessor(p), valueTreeState(vts)
{
    // Sliders
    addAndMakeVisible(volumeSlider);
    volumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    volumeSlider.setTextValueSuffix("%");
    volumeSlider.setRange(0.0, 100.0, 1.00);
    volumeAttachment.reset(new SliderAttachment(valueTreeState, "volume", volumeSlider));

    // Labels
    addAndMakeVisible(volumeLabel);
    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeLabel.setJustificationType(juce::Justification::centredTop);
    volumeLabel.attachToComponent(&volumeSlider, false);

    // Size
    setSize(80, 120);
}

VolumeComponent::~VolumeComponent()
{
}

void VolumeComponent::paint(juce::Graphics& g)
{
    if constexpr (Config::BORDER > 0)
    {
        juce::Rectangle<int> rect(0, 0, getWidth(), getHeight());
        g.setColour(Config::getBorderColor());
        g.drawRect(rect, Config::BORDER);
    }
}

void VolumeComponent::resized()
{
    volumeSlider.setBounds(Config::PADDING, Config::PADDING + Config::TEXTBOX_HEIGHT, getWidth() - (2*Config::PADDING), getHeight() - Config::PADDING - (Config::PADDING + Config::TEXTBOX_HEIGHT));
}