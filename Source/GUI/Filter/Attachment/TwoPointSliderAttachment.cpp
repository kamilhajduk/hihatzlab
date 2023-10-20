#include "TwoPointSliderAttachment.h"

TwoValueSliderAttachment::TwoValueSliderAttachment(juce::AudioProcessorValueTreeState& vts, const juce::String minParamID, const juce::String maxParamID, juce::Slider& slider) :
    twoPointSlider(slider),
    minValue(*vts.getParameter(minParamID), [this](float value) {
            twoPointSlider.setMinValue(static_cast<double>(value));
        }),
    maxValue(*vts.getParameter(maxParamID), [this](float value) {
            twoPointSlider.setMaxValue(static_cast<double>(value));
        })
{

    auto minParam = vts.getParameter(minParamID);
    auto maxParam = vts.getParameter(maxParamID);

    // Expect minParam == maxParam
    jassert(minParam->getNormalisableRange().start == maxParam->getNormalisableRange().start
        && minParam->getNormalisableRange().end
        == maxParam->getNormalisableRange().end);

    auto minDefault = minParam->convertFrom0to1(minParam->getDefaultValue());
    auto maxDefault = maxParam->convertFrom0to1(maxParam->getDefaultValue());

    slider.setRange(minParam->getNormalisableRange().start, minParam->getNormalisableRange().end);
    slider.setMinAndMaxValues(static_cast<double>(minDefault), static_cast<double>(maxDefault));

    slider.onDragStart = [&]
    {
        lastDraggedThumb = slider.getThumbBeingDragged();
        if (lastDraggedThumb == 1) minValue.beginGesture();
        else if (lastDraggedThumb == 2) maxValue.beginGesture();
        slider.updateText();
    };
    slider.onDragEnd = [&]
    {
        if (lastDraggedThumb == 1) minValue.endGesture();
        else if (lastDraggedThumb == 2) maxValue.endGesture();
        lastDraggedThumb = 0;
        slider.updateText();
    };

    slider.valueFromTextFunction = [&](const juce::String& text) {
        int delimiterIndex = text.indexOfChar('-');
        if (delimiterIndex != -1)
        {
            juce::String minValue = text.substring(0, delimiterIndex).trim();
            juce::String maxValue = text.substring(delimiterIndex + 1).trim();

            slider.setMinValue(minValue.getDoubleValue(), juce::sendNotification);
            slider.setMaxValue(maxValue.getDoubleValue(), juce::sendNotification);
        }

        return 0.0;
    };

    slider.onValueChange = [&]
    {
        if (lastDraggedThumb == 1) minValue.setValueAsCompleteGesture(static_cast<float>(slider.getMinValue()));
        else if (lastDraggedThumb == 2) maxValue.setValueAsCompleteGesture(static_cast<float>(slider.getMaxValue()));
        slider.updateText();
    };

    slider.textFromValueFunction = [&](double input)
    {
        juce::ignoreUnused(input);
        juce::String output = juce::String(slider.getMinValue(), 0);
        output += " - ";
        output += juce::String(slider.getMaxValue(), 0);
        return output;
    };
}