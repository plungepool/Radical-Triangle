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
    oscWaveR.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber - 48));
    adsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff) 
{
    adsr.noteOff();
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
    gain.setGainLinear(0.01f);

    isPrepared = true;
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) 
{
    jassert(isPrepared);

    juce::dsp::AudioBlock<float> audioBlock{ outputBuffer };
    oscWaveL.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));
    oscWaveR.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    //contextreplacing lets you replace previous process block after it's finished

    adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);

}