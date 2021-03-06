/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class RadTriAudioProcessorEditor :  public juce::AudioProcessorEditor,
                                    public juce::Timer, //60hz refresh for animation
                                    public juce::Slider::Listener //adds listeners to sliders
{
public:
    RadTriAudioProcessorEditor (RadTriAudioProcessor&);
    ~RadTriAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;

    const float pi = juce::MathConstants<float>::pi;

private:
    int windowWidth = 400;
    int windowHeight = 400;

    const juce::ColourGradient bgGradient = juce::ColourGradient(juce::Colours::purple, 1, 300, juce::Colours::orange, 300, 1, true);

    juce::Slider wideSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wideSliderAttachment;
    juce::Slider satSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> satSliderAttachment;

    juce::Slider atkSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> atkSliderAttachment;
    juce::Slider dcySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dcySliderAttachment;
    juce::Slider susSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> susSliderAttachment;
    juce::Slider relSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> relSliderAttachment;

    // Graphics
    unsigned char cRTriL = 248;
    unsigned char cGTriL = 248;
    unsigned char cBTriL = 255;
    unsigned char cRTriR = 255;
    unsigned char cGTriR = 248;
    unsigned char cBTriR = 248;
    float transpTri = 1.0f;
    float scaleTri = 1.0f;
    juce::Colour triColor = juce::Colour::Colour(cRTriL, cGTriL, cBTriL, transpTri);

    float tWidthL = (wideSlider.getValue()) / -2;
    float tWidthR = (wideSlider.getValue()) / 2;

    float tSatLX = (satSlider.getValue()) * -4;
    float tSatLY = (satSlider.getValue()) * -10;
    float tSatRX = (satSlider.getValue()) * -4;
    float tSatRY = (satSlider.getValue()) * -10;
    float sSatL = (satSlider.getValue()) / 2;
    float sSatR = (satSlider.getValue()) / 2;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RadTriAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RadTriAudioProcessorEditor)
};
