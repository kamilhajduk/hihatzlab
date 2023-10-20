#include "ADSRComponent.h"

ADSRComponent::ADSRComponent(HiHatzLabAudioProcessor& p, juce::AudioProcessorValueTreeState& vts) : audioProcessor(p), valueTreeState(vts)
{
    // Sliders
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(sustainSlider);
    addAndMakeVisible(releaseSlider);

    attackSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    decaySlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    sustainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);

    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, Config::TEXTBOX_WIDTH, Config::TEXTBOX_HEIGHT);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, Config::TEXTBOX_WIDTH, Config::TEXTBOX_HEIGHT);
    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, Config::TEXTBOX_WIDTH, Config::TEXTBOX_HEIGHT);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, Config::TEXTBOX_WIDTH, Config::TEXTBOX_HEIGHT);

    attackAttachment.reset(new SliderAttachment(valueTreeState, "attack", attackSlider));
    decayAttachment.reset(new SliderAttachment(valueTreeState, "decay", decaySlider));
    sustainAttachment.reset(new SliderAttachment(valueTreeState, "sustain", sustainSlider));
    releaseAttachment.reset(new SliderAttachment(valueTreeState, "release", releaseSlider));

    attackSlider.setRange(0.0, 5.0, 0.01);
    decaySlider.setRange(0.0, 5.0, 0.01);
    sustainSlider.setRange(0.0, 1.0, 0.01);
    releaseSlider.setRange(0.0, 5.0, 0.01);

    // If any ADSR knob value change, replace ADSR parameters in audio processor with new value
    attackSlider.onValueChange = [this] {
        juce::ADSR::Parameters parameters = audioProcessor.getAdsrParameters();
        parameters.attack = static_cast<float>(attackSlider.getValue());
        audioProcessor.setAdsrParameters(parameters);
    };

    decaySlider.onValueChange = [this] {
        juce::ADSR::Parameters parameters = audioProcessor.getAdsrParameters();
        parameters.decay = static_cast<float>(decaySlider.getValue());
        audioProcessor.setAdsrParameters(parameters);
    };

    sustainSlider.onValueChange = [this] {
        juce::ADSR::Parameters parameters = audioProcessor.getAdsrParameters();
        parameters.sustain = static_cast<float>(sustainSlider.getValue());
        audioProcessor.setAdsrParameters(parameters);
    };

    releaseSlider.onValueChange = [this] {
        juce::ADSR::Parameters parameters = audioProcessor.getAdsrParameters();
        parameters.release = static_cast<float>(releaseSlider.getValue());
        audioProcessor.setAdsrParameters(parameters);
    };

    // Labels
    addAndMakeVisible(attackLabel);
    addAndMakeVisible(decayLabel);
    addAndMakeVisible(sustainLabel);
    addAndMakeVisible(releaseLabel);

    attackLabel.setText("Attack", juce::dontSendNotification);
    attackLabel.setJustificationType(juce::Justification::centredTop);
    attackLabel.attachToComponent(&attackSlider, false);

    decayLabel.setText("Decay", juce::dontSendNotification);
    decayLabel.setJustificationType(juce::Justification::centredTop);
    decayLabel.attachToComponent(&decaySlider, false);

    sustainLabel.setText("Sustain", juce::dontSendNotification);
    sustainLabel.setJustificationType(juce::Justification::centredTop);
    sustainLabel.attachToComponent(&sustainSlider, false);

    releaseLabel.setText("Release", juce::dontSendNotification);
    releaseLabel.setJustificationType(juce::Justification::centredTop);
    releaseLabel.attachToComponent(&releaseSlider, false);

    // Size
    setSize (250, 120);
}

ADSRComponent::~ADSRComponent()
{
}

void ADSRComponent::paint (juce::Graphics& g)
{
    if constexpr (Config::BORDER > 0)
    {
        juce::Rectangle<int> rect(0, 0, getWidth(), getHeight());
        g.setColour(Config::getBorderColor());
        g.drawRect(rect, Config::BORDER);
    }
}

void ADSRComponent::resized()
{
    attackSlider.setBounds(    Config::PADDING                        , Config::PADDING + Config::TEXTBOX_HEIGHT, Config::KNOB_WIDTH, getHeight() - Config::PADDING - (Config::PADDING + Config::TEXTBOX_HEIGHT));
    decaySlider.setBounds(  2* Config::PADDING +    Config::KNOB_WIDTH, Config::PADDING + Config::TEXTBOX_HEIGHT, Config::KNOB_WIDTH, getHeight() - Config::PADDING - (Config::PADDING + Config::TEXTBOX_HEIGHT));
    sustainSlider.setBounds(3* Config::PADDING + 2* Config::KNOB_WIDTH, Config::PADDING + Config::TEXTBOX_HEIGHT, Config::KNOB_WIDTH, getHeight() - Config::PADDING - (Config::PADDING + Config::TEXTBOX_HEIGHT));
    releaseSlider.setBounds(4* Config::PADDING + 3* Config::KNOB_WIDTH, Config::PADDING + Config::TEXTBOX_HEIGHT, Config::KNOB_WIDTH, getHeight() - Config::PADDING - (Config::PADDING + Config::TEXTBOX_HEIGHT));
}
