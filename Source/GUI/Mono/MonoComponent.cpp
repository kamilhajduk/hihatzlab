#include "MonoComponent.h"

MonoComponent::MonoComponent(HiHatzLabAudioProcessor& p, juce::AudioProcessorValueTreeState& vts) : audioProcessor(p), valueTreeState(vts)
{
    addAndMakeVisible(monoButton);
    monoButton.setButtonText("Mono");
    monoAttachment.reset(new ButtonAttachment(valueTreeState, "mono", monoButton));

    // Size
    setSize(80, 40);
}

MonoComponent::~MonoComponent()
{
}

void MonoComponent::paint(juce::Graphics& g)
{
    if constexpr (Config::BORDER > 0)
    {
        juce::Rectangle<int> rect(0, 0, getWidth(), getHeight());
        g.setColour(Config::getBorderColor());
        g.drawRect(rect, Config::BORDER);
    }
}

void MonoComponent::resized()
{
    monoButton.setBounds(Config::PADDING, Config::PADDING, getWidth() - 2*Config::PADDING, getHeight() - 2*Config::PADDING);
}