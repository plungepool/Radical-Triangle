/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RadTriAudioProcessorEditor::RadTriAudioProcessorEditor (RadTriAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    oscPhase.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    oscPhase.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    oscPhase.setRange(-180.0, 180.0, 1.0);
    oscPhase.setValue(0.0);
    oscPhase.addListener(this);
    addAndMakeVisible(oscPhase);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

RadTriAudioProcessorEditor::~RadTriAudioProcessorEditor()
{
}

//==============================================================================
void RadTriAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)

    g.fillAll(juce::Colours::purple);

    g.setGradientFill(bgGradient);
    g.fillAll();

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Phase Offset", getLocalBounds(), juce::Justification::left, 1);
}

void RadTriAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    oscPhase.setBounds(0, 0, 100, 100);
}

void RadTriAudioProcessorEditor::sliderValueChanged(juce::Slider *slider) {
    if (slider == &oscPhase) {
        audioProcessor.phaseValue = oscPhase.getValue();
    }
}