/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RadTriAudioProcessorEditor::RadTriAudioProcessorEditor (RadTriAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), Timer()
{
    oscPhase.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    oscPhase.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    juce::AudioProcessorEditor::addAndMakeVisible(oscPhase);
    oscPhaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "WIDENER", oscPhase);
    oscPhase.addListener(this);

    satSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    satSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    juce::AudioProcessorEditor::addAndMakeVisible(satSlider);
    satSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "SATURATION", satSlider);
    satSlider.addListener(this);

    atkSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    atkSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    juce::AudioProcessorEditor::addAndMakeVisible(atkSlider);
    atkSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ATTACK", atkSlider);
    //atkSlider.addListener(this);

    dcySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    dcySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    juce::AudioProcessorEditor::addAndMakeVisible(dcySlider);
    dcySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DECAY", dcySlider);
    //dcySlider.addListener(this);

    susSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    susSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    juce::AudioProcessorEditor::addAndMakeVisible(susSlider);
    susSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "SUSTAIN", susSlider);
    //susSlider.addListener(this);

    relSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    relSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    juce::AudioProcessorEditor::addAndMakeVisible(relSlider);
    relSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RELEASE", relSlider);
    //relSlider.addListener(this);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    juce::AudioProcessorEditor::setSize(windowWidth, windowHeight);
    startTimerHz(60);
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

    // Triangle Scaling
    scaleTri = (audioProcessor.rms + 0.08f) * 10.0f;
    transpTri += (audioProcessor.rms * 10);

    // Left Channel Triangle
    triColor = juce::Colour::Colour(cRTriL, cGTriL, cBTriL, transpTri);
    g.setColour(triColor);

    tWidthL = (oscPhase.getValue()) / 2;
    tSatLX = (satSlider.getValue()) * -4;
    tSatLY = (satSlider.getValue()) * -14;
    sSatL = (satSlider.getValue()) / 3;

    juce::Path gTriL;
    gTriL.addTriangle  (windowWidth / 2, windowHeight / 4,
                        windowWidth / 5, (windowHeight / 4) * 3,
                        (windowWidth / 5) * 4, (windowHeight / 4) * 3);
    gTriL.applyTransform(juce::AffineTransform::translation(tWidthL, 0));
    gTriL.applyTransform(juce::AffineTransform::scale(scaleTri, scaleTri, windowWidth/2, windowHeight / 2));
    g.strokePath(gTriL, juce::PathStrokeType(10.6f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));

    juce::Path gEyeL;
    gEyeL.startNewSubPath((windowWidth / 100) * 35, (windowHeight / 100) * 58);
    gEyeL.quadraticTo    (windowWidth / 2, (windowHeight / 100) * 43,
                        (windowWidth / 100) * 65, (windowHeight / 100) * 58);
    gEyeL.quadraticTo    (windowWidth / 2, (windowHeight / 100) * 70,
                        (windowWidth / 100) * 35, (windowHeight / 100) * 58);
    gEyeL.applyTransform(juce::AffineTransform::translation(tWidthL, 0));
    gEyeL.applyTransform(juce::AffineTransform::scale(scaleTri, scaleTri, windowWidth / 2, windowHeight / 2));
    g.strokePath(gEyeL, juce::PathStrokeType(10.6f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));

    juce::Path gPupilL;
    gPupilL.startNewSubPath  ((windowWidth / 100) * 46, (windowHeight / 100) * 61);
    gPupilL.quadraticTo      ((windowWidth / 100) * 51, (windowHeight / 100) * 54,
                            (windowWidth / 100) * 57, (windowHeight / 100) * 61);
    gPupilL.quadraticTo      ((windowWidth / 100) * 51, (windowHeight / 100) * 66,
                            (windowWidth / 100) * 46, (windowHeight / 100) * 61);
    gPupilL.applyTransform(juce::AffineTransform::scale(1 + sSatL, 1 + sSatL, windowWidth / 1.9, windowHeight / 1.7));
    gPupilL.applyTransform(juce::AffineTransform::translation(tWidthL + tSatLX, tSatLY));
    gPupilL.applyTransform(juce::AffineTransform::scale(scaleTri, scaleTri, windowWidth / 2, windowHeight / 2));
    g.strokePath(gPupilL, juce::PathStrokeType(10.6f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));

    // Right Channel Triangle
    triColor = juce::Colour::Colour(cRTriR, cGTriR, cBTriR, transpTri);
    g.setColour(triColor);

    tWidthR = (oscPhase.getValue()) / -2;
    tSatRX = (satSlider.getValue()) * -4;
    tSatRY = (satSlider.getValue()) * -14;
    sSatR = (satSlider.getValue()) / 3;

    juce::Path gTriR;
    gTriR.addTriangle(windowWidth / 2, windowHeight / 4,
        windowWidth / 5, (windowHeight / 4) * 3,
        (windowWidth / 5) * 4, (windowHeight / 4) * 3);
    gTriR.applyTransform(juce::AffineTransform::translation(tWidthR, 0));
    gTriR.applyTransform(juce::AffineTransform::scale(scaleTri, scaleTri, windowWidth / 2, windowHeight / 2));
    g.strokePath(gTriR, juce::PathStrokeType(10.6f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));

    juce::Path gEyeR;
    gEyeR.startNewSubPath((windowWidth / 100) * 35, (windowHeight / 100) * 58);
    gEyeR.quadraticTo(windowWidth / 2, (windowHeight / 100) * 43,
        (windowWidth / 100) * 65, (windowHeight / 100) * 58);
    gEyeR.quadraticTo(windowWidth / 2, (windowHeight / 100) * 70,
        (windowWidth / 100) * 35, (windowHeight / 100) * 58);
    gEyeR.applyTransform(juce::AffineTransform::translation(tWidthR, 0));
    gEyeR.applyTransform(juce::AffineTransform::scale(scaleTri, scaleTri, windowWidth / 2, windowHeight / 2));
    g.strokePath(gEyeR, juce::PathStrokeType(10.6f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));

    juce::Path gPupilR;
    gPupilR.startNewSubPath((windowWidth / 100) * 46, (windowHeight / 100) * 61);
    gPupilR.quadraticTo((windowWidth / 100) * 51, (windowHeight / 100) * 54,
        (windowWidth / 100) * 57, (windowHeight / 100) * 61);
    gPupilR.quadraticTo((windowWidth / 100) * 51, (windowHeight / 100) * 66,
        (windowWidth / 100) * 46, (windowHeight / 100) * 61);
    gPupilR.applyTransform(juce::AffineTransform::scale(1 + sSatR, 1 + sSatR, windowWidth / 1.9, windowHeight / 1.7));
    gPupilR.applyTransform(juce::AffineTransform::translation(tWidthR + tSatRX, tSatRY));
    gPupilR.applyTransform(juce::AffineTransform::scale(scaleTri, scaleTri, windowWidth / 2, windowHeight / 2));
    g.strokePath(gPupilR, juce::PathStrokeType(10.6f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));
}

void RadTriAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    oscPhase.setBounds(0, 0, 100, 100);
    satSlider.setBounds(windowWidth - 100, 0, 100, 100);

    atkSlider.setBounds((windowWidth / 10) * 1, windowHeight - 75, 75, 75);
    dcySlider.setBounds((windowWidth / 10) * 3, windowHeight - 75, 75, 75);
    susSlider.setBounds((windowWidth / 10) * 5, windowHeight - 75, 75, 75);
    relSlider.setBounds((windowWidth / 10) * 7, windowHeight - 75, 75, 75);

}

void RadTriAudioProcessorEditor::sliderValueChanged(juce::Slider *slider) {
    juce::AudioProcessorEditor::repaint(0, 0, windowWidth, windowHeight);
}

void RadTriAudioProcessorEditor::timerCallback() {
    juce::AudioProcessorEditor::repaint(0, 0, windowWidth, windowHeight);
}
