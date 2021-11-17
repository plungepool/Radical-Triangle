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
    oscWaveR.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    oscWaveSub.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber - 24));
    oscWaveSup.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber + 12));
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
    oscWaveSub.prepare(spec);
    oscWaveSup.prepare(spec);

    supGain.prepare(spec);
    supGain.setGainLinear(0.05f);

    subGain.prepare(spec);
    subGain.setGainLinear(0.05f);

    mainGain.prepare(spec);
    mainGain.setGainLinear(0.05f);

    saturation.prepare(satSpec);

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
    oscWaveSup.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    supGain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    oscWaveSub.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    subGain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    oscWaveL.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));
    oscWaveR.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    mainGain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    //saturation process goes here
    //saturation.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    //contextreplacing lets you replace previous process block after it's finished

    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
        if (!adsr.isActive()) {
            clearCurrentNote();
        }
    }
}

void SynthVoice::updateSupGain(const float sup) {
    supGain.setGainLinear(sup / 8);
}

void SynthVoice::updateSubGain(const float sub) {
    subGain.setGainLinear(sub / 4);
}

void SynthVoice::updateSaturationAmount(const float sat) {
    //saturation.process(sat);
}