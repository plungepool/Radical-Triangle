/*
  ==============================================================================

    SynthVoice.cpp
    Created: 7 Jun 2021 6:14:09pm
    Author:  cyclpsrock

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) 
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) 
{
    oscWaveL.setFrequency(juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber));
    oscWaveR.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber - 24));
    adsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff) 
{
    adsr.noteOff();

    if (!allowTailOff || !adsr.isActive()) {
        clearCurrentNote();
    }
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) 
{

}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue) 
{

}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) 
{
    adsr.setSampleRate(sampleRate);

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    oscWaveL.prepare(spec);
    oscWaveR.prepare(spec);

    gain.prepare(spec);
    gain.setGainLinear(0.03f);

    isPrepared = true;
}

void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release) {
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;

    adsr.setParameters(adsrParams);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) 
{
    jassert(isPrepared);

    if (!isVoiceActive()) {
        return;
    }

    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();

    juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };
    oscWaveL.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));
    oscWaveR.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    //contextreplacing lets you replace previous process block after it's finished

    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

    //if (startSample != 0) {
    //    jassertfalse;
    //}

    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
        if (!adsr.isActive()) {
            clearCurrentNote();
        }
    }

}