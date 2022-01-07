/*
  ==============================================================================

    SynthVoice.h
    Created: 7 Jun 2021 6:14:09pm
    Author:  cyclpsrock

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include <math.h>

class SynthVoice : public juce::SynthesiserVoice {
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void updateADSR(const float attack, const float decay, const float sustain, const float release);
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

    void updateSupGain(const float sup);
    void updateSubGain(const float sub);

private:
    //OSCILLATORS
    juce::dsp::Oscillator<float> oscWaveL{ [](float x) { return 1 - 2 * abs(x / juce::MathConstants<float>::pi); }, 200 };
    juce::dsp::Oscillator<float> oscWaveR{ [](float x) { return 1 - 2 * abs(x / juce::MathConstants<float>::pi); }, 200 };
    juce::dsp::Oscillator<float> oscWaveSub{ [](float x) { return 1 - 2 * abs(x / juce::MathConstants<float>::pi); }, 200 };
    juce::dsp::Oscillator<float> oscWaveSup{ [](float x) { return 1 - 2 * abs(x / juce::MathConstants<float>::pi); }, 200 };

    //OSC GAIN STRUCTURE
    juce::dsp::Gain<float> supGain;
    juce::dsp::Gain<float> subGain;
    juce::dsp::Gain<float> mainGain;

    //ADSR
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    juce::AudioBuffer<float> synthBuffer;

    bool isPrepared{ false };
};