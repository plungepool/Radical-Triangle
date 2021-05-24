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
    setSize(400, 400);
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
    //g.setOrigin(10, 100);
    g.drawFittedText ("Phase Offset", 10, 100, 80, 20, juce::Justification::verticallyCentred, 1);

    //Paint just constantly redraws line so should be able to make
    //em variable and sort of animate them?
    //g.setColour(juce::Colours::white);
    //juce::Line<float> line1 (juce::Point<float>((getWidth() / 2), (getHeight() / 3)),
    //                        juce::Point<float>((getWidth() / 4), (getHeight() / 3) * 2));
    //g.drawLine(line1, 8.0f);
    //juce::Line<float> line2 (juce::Point<float>((getWidth() / 4), (getHeight() / 3) * 2),
    //                        juce::Point<float>((getWidth() / 4) * 3, (getHeight() / 3) * 2));
    //g.drawLine(line2, 8.0f);
    //juce::Line<float> line3 (juce::Point<float>((getWidth() / 4) * 3, (getHeight() / 3) * 2),
    //                        juce::Point<float>((getWidth() / 2), (getHeight() / 3)));
    //g.drawLine(line3, 8.0f);

    //myPath.startNewSubPath((getWidth() / 2), (getHeight() / 3));          // move the current position to (10, 10)
    //myPath.lineTo(getWidth() / 4, (getHeight() / 3) * 2);                 // draw a line from here to (100, 200)
    //myPath.lineTo((getWidth() / 4) * 3, (getHeight() / 3) * 2);
    //myPath.quadraticTo(300.0f, 300.0f, 150.0f, 50.0f); // draw a curve that ends at (5, 50)
    //myPath.closeSubPath();                          // close the subpath with a line back to (10, 10)

    //myPath.addArc       (getWidth() / 3, getHeight() / 3,
    //                    (getWidth() / 3) * 2, (getHeight() / 3) * 2, 
    //                    0, pi, true);

    // add an ellipse as well, which will form a second sub-path within the path..
    //myPath.addEllipse(getWidth() / 2, getHeight() / 2, 40.0f, 30.0f);

    // double the width of the whole thing..
    //myPath.applyTransform(juce::AffineTransform::scale(1.2f, 1.2f));

    juce::Path gTri;
    gTri.addTriangle  (getWidth() / 2, getHeight() / 4,
                        getWidth() / 5, (getHeight() / 4) * 3, 
                        (getWidth() / 5) * 4, (getHeight() / 4) * 3);
    g.strokePath(gTri, juce::PathStrokeType(10.6f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));

    juce::Path gEye;
    gEye.startNewSubPath((getWidth() / 100) * 35, (getHeight() / 100) * 58);
    gEye.quadraticTo    (getWidth() / 2, (getHeight() / 100) * 43,
                        (getWidth() / 100) * 65, (getHeight() / 100) * 58);
    gEye.quadraticTo    (getWidth() / 2, (getHeight() / 100) * 70,
                        (getWidth() / 100) * 35, (getHeight() / 100) * 58);
    g.strokePath(gEye, juce::PathStrokeType(10.6f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));

    juce::Path gPupil;
    gPupil.startNewSubPath  ((getWidth() / 100) * 46, (getHeight() / 100) * 61);
    gPupil.quadraticTo      ((getWidth() / 100) * 51, (getHeight() / 100) * 54,
                            (getWidth() / 100) * 57, (getHeight() / 100) * 61);
    gPupil.quadraticTo      ((getWidth() / 100) * 51, (getHeight() / 100) * 66,
                            (getWidth() / 100) * 46, (getHeight() / 100) * 61);
    g.strokePath(gPupil, juce::PathStrokeType(10.6f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));

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