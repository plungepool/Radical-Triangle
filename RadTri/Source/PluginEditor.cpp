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
    oscPhaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "WIDENER", oscPhase);
    oscPhase.addListener(this);

    satSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    satSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible(satSlider);
    satSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "SATURATION", satSlider);
    satSlider.addListener(this);

    atkSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    atkSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible(atkSlider);
    atkSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ATTACK", atkSlider);
    //atkSlider.addListener(this);

    dcySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    dcySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible(dcySlider);
    dcySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DECAY", dcySlider);
    //dcySlider.addListener(this);

    susSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    susSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible(susSlider);
    susSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "SUSTAIN", susSlider);
    //susSlider.addListener(this);

    relSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    relSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible(relSlider);
    relSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RELEASE", relSlider);
    //relSlider.addListener(this);

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
    //g.setOrigin(0, 0);

    g.fillAll(juce::Colours::purple);

    g.setGradientFill(bgGradient);
    g.fillAll();

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Widener", 10, 100, 80, 20, juce::Justification::verticallyCentred, 1);

    // Triangle Color/Opacity
    if (oscPhase.getValue() == 0) {
        transpTri = 1.0f;
    }
    else if (oscPhase.getValue() < 0) {
        transpTri = (-1 / oscPhase.getValue()) * 40;
    }
    else {
        transpTri = (1 / oscPhase.getValue()) * 40;
    }

    // Left Channel Triangle
    triColor = juce::Colour::Colour(cRTriL, cGTriL, cBTriL, transpTri);
    g.setColour(triColor);

    tWidthL = (oscPhase.getValue()) / 2;
    tSatLX = (satSlider.getValue()) * -4;
    tSatLY = (satSlider.getValue()) * -14;
    sSatL = (satSlider.getValue()) / 3;

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
    gPupilL.applyTransform(juce::AffineTransform::scale(1 + sSatL, 1 + sSatL, getWidth() / 1.9, getHeight() / 1.7));
    gPupilL.applyTransform(juce::AffineTransform::translation(tWidthL + tSatLX, tSatLY));
    g.strokePath(gPupilL, juce::PathStrokeType(10.6f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));

    // Right Channel Triangle
    triColor = juce::Colour::Colour(cRTriR, cGTriR, cBTriR, transpTri);
    g.setColour(triColor);

    tWidthR = (oscPhase.getValue()) / -2;
    tSatRX = (satSlider.getValue()) * -4;
    tSatRY = (satSlider.getValue()) * -14;
    sSatR = (satSlider.getValue()) / 3;

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
    gPupilR.applyTransform(juce::AffineTransform::scale(1 + sSatR, 1 + sSatR, getWidth() / 1.9, getHeight() / 1.7));
    gPupilR.applyTransform(juce::AffineTransform::translation(tWidthR + tSatRX, tSatRY));
    g.strokePath(gPupilR, juce::PathStrokeType(10.6f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));
}

void RadTriAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    oscPhase.setBounds(0, 0, 100, 100);
    satSlider.setBounds(getWidth() - 100, 0, 100, 100);

    atkSlider.setBounds((getWidth() / 10) * 1, getHeight() - 75, 75, 75);
    dcySlider.setBounds((getWidth() / 10) * 3, getHeight() - 75, 75, 75);
    susSlider.setBounds((getWidth() / 10) * 5, getHeight() - 75, 75, 75);
    relSlider.setBounds((getWidth() / 10) * 7, getHeight() - 75, 75, 75);
}

void RadTriAudioProcessorEditor::sliderValueChanged(juce::Slider *slider) {
    repaint(0, 0, windowWidth, windowHeight);
}