#include "BPMComponent.h"

BPMComponent::BPMComponent(HiHatzLabAudioProcessor& p, juce::AudioProcessorValueTreeState& vts) : audioProcessor(p), valueTreeState(vts)
{
    // Sliders
    addAndMakeVisible(bpmSlider);
    bpmSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    bpmSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, Config::TEXTBOX_WIDTH, Config::TEXTBOX_HEIGHT);
    bpmAttachment.reset(new SliderAttachment(valueTreeState, "bpm", bpmSlider));
    bpmSlider.setRange(10.0, 522.0, 1.00);

    // Labels
    addAndMakeVisible(bpmLabel);

    bpmLabel.setText("BPM", juce::dontSendNotification);
    bpmLabel.setJustificationType(juce::Justification::centredTop);
    bpmLabel.attachToComponent(&bpmSlider, false);

    // Size
    setSize(80, 70);
}

BPMComponent::~BPMComponent()
{
}

void BPMComponent::paint(juce::Graphics& g)
{
    if constexpr (Config::BORDER > 0)
    {
        juce::Rectangle<int> rect(0, 0, getWidth(), getHeight());
        g.setColour(Config::getBorderColor());
        g.drawRect(rect, Config::BORDER);
    }
}

void BPMComponent::resized()
{
    bpmSlider.setBounds(Config::PADDING, Config::PADDING + Config::TEXTBOX_HEIGHT, getWidth() - 2*Config::PADDING, getHeight() - Config::PADDING - (Config::PADDING + Config::TEXTBOX_HEIGHT));
}