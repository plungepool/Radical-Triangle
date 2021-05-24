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
                                    public juce::Slider::Listener //add listener to sliders
{
public:
    RadTriAudioProcessorEditor (RadTriAudioProcessor&);
    ~RadTriAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;

    const float pi = juce::MathConstants<float>::pi;

private:
    const juce::ColourGradient bgGradient = juce::ColourGradient(juce::Colours::purple, 1, 300, juce::Colours::orange, 300, 1, true);

    juce::Slider oscPhase;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RadTriAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RadTriAudioProcessorEditor)
};
