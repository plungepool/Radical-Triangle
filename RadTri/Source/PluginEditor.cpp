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
    addAndMakeVisible(oscPhase);
    oscPhaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PHASE_OFFSET", oscPhase);
    oscPhase.addListener(this);

    gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible(gainSlider);
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);
    gainSlider.addListener(this);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(windowWidth, windowHeight);
}

RadTriAudioProcessorEditor::~RadTriAudioProcessorEditor()
{
}

//==============================================================================

void RadTriAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::purple);

    g.setGradientFill(bgGradient);
    g.fillAll();

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.setOrigin(10, 100);
    g.drawFittedText ("Phase Offset", 10, 100, 80, 20, juce::Justification::verticallyCentred, 1);

    // Triangle Color/Opacity
    if (oscPhase.getValue() == 0) {
        transpTri = 1.0f;
    }
    else if (oscPhase.getValue() < 0) {
        transpTri = (-1 / oscPhase.getValue()) * 35;
    }
    else {
        transpTri = (1 / oscPhase.getValue()) * 35;
    }
    triColor = juce::Colour::Colour(cRTri, cGTri, cBTri, transpTri);
    g.setColour(triColor);

    // Left Channel Triangle
    tWidthL = (oscPhase.getValue()) / 2;

    juce::Path gTriL;
    gTriL.addTriangle  (getWidth() / 2, getHeight() / 4,
                        getWidth() / 5, (getHeight() / 4) * 3, 
                        (getWidth() / 5) * 4, (getHeight() / 4) * 3);
    gTriL.applyTransform(juce::AffineTransform::translation(tWidthL, 0));
    g.strokePath(gTriL, juce::PathStrokeType(10.6f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));

    juce::Path gEyeL;
    gEyeL.startNewSubPath((getWidth() / 100) * 35, (getHeight() / 100) * 58);
    gEyeL.quadraticTo    (getWidth() / 2, (getHeight() / 100) * 43,
                        (getWidth() / 100) * 65, (getHeight() / 100) * 58);
    gEyeL.quadraticTo    (getWidth() / 2, (getHeight() / 100) * 70,
                        (getWidth() / 100) * 35, (getHeight() / 100) * 58);
    gEyeL.applyTransform(juce::AffineTransform::translation(tWidthL, 0));
    g.strokePath(gEyeL, juce::PathStrokeType(10.6f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));

    juce::Path gPupilL;
    gPupilL.startNewSubPath  ((getWidth() / 100) * 46, (getHeight() / 100) * 61);
    gPupilL.quadraticTo      ((getWidth() / 100) * 51, (getHeight() / 100) * 54,
                            (getWidth() / 100) * 57, (getHeight() / 100) * 61);
    gPupilL.quadraticTo      ((getWidth() / 100) * 51, (getHeight() / 100) * 66,
                            (getWidth() / 100) * 46, (getHeight() / 100) * 61);
    gPupilL.applyTransform(juce::AffineTransform::translation(tWidthL, 0));
    g.strokePath(gPupilL, juce::PathStrokeType(10.6f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));

    // Right Channel Triangle
    tWidthR = (oscPhase.getValue()) / -2;

    juce::Path gTriR;
    gTriR.addTriangle(getWidth() / 2, getHeight() / 4,
        getWidth() / 5, (getHeight() / 4) * 3,
        (getWidth() / 5) * 4, (getHeight() / 4) * 3);
    gTriR.applyTransform(juce::AffineTransform::translation(tWidthR, 0));
    g.strokePath(gTriR, juce::PathStrokeType(10.6f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));

    juce::Path gEyeR;
    gEyeR.startNewSubPath((getWidth() / 100) * 35, (getHeight() / 100) * 58);
    gEyeR.quadraticTo(getWidth() / 2, (getHeight() / 100) * 43,
        (getWidth() / 100) * 65, (getHeight() / 100) * 58);
    gEyeR.quadraticTo(getWidth() / 2, (getHeight() / 100) * 70,
        (getWidth() / 100) * 35, (getHeight() / 100) * 58);
    gEyeR.applyTransform(juce::AffineTransform::translation(tWidthR, 0));
    g.strokePath(gEyeR, juce::PathStrokeType(10.6f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));

    juce::Path gPupilR;
    gPupilR.startNewSubPath((getWidth() / 100) * 46, (getHeight() / 100) * 61);
    gPupilR.quadraticTo((getWidth() / 100) * 51, (getHeight() / 100) * 54,
        (getWidth() / 100) * 57, (getHeight() / 100) * 61);
    gPupilR.quadraticTo((getWidth() / 100) * 51, (getHeight() / 100) * 66,
        (getWidth() / 100) * 46, (getHeight() / 100) * 61);
    gPupilR.applyTransform(juce::AffineTransform::translation(tWidthR, 0));
    g.strokePath(gPupilR, juce::PathStrokeType(10.6f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));
}

void RadTriAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    oscPhase.setBounds(0, 0, 100, 100);
    gainSlider.setBounds(getWidth() - 100, 0, 100, 100);
}

void RadTriAudioProcessorEditor::sliderValueChanged(juce::Slider *slider) {
    repaint(0, 0, windowWidth, windowHeight);
}