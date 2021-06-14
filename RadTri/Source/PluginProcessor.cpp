/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RadTriAudioProcessor::RadTriAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), false)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());

}

RadTriAudioProcessor::~RadTriAudioProcessor()
{
    //Processor destructor
}

//==============================================================================
const juce::String RadTriAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RadTriAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RadTriAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RadTriAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double RadTriAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RadTriAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RadTriAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RadTriAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String RadTriAudioProcessor::getProgramName (int index)
{
    return {};
}

void RadTriAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void RadTriAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    // Calls anytime the user presses play

    synth.setCurrentPlaybackSampleRate(sampleRate);
    
    for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void RadTriAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RadTriAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void RadTriAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //Where audio is processed

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int i = 0; i < synth.getNumVoices(); ++i) {
        //check for changes in ADSR
        if (auto voice = dynamic_cast<juce::SynthesiserVoice*>(synth.getVoice(i))) {
            //Osc Controls
            //ADSR
            //lfo, etc
        }
    }

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    //for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //{
    //    auto* channelData = buffer.getWritePointer (channel);
    //    // ..do something to the data...

    //    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
    //        //channelData[sample] = channelData[sample] * juce::Decibels::decibelsToGain();
    //    }
    //}
}

//==============================================================================
bool RadTriAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* RadTriAudioProcessor::createEditor()
{
    return new RadTriAudioProcessorEditor (*this);
}

//==============================================================================
void RadTriAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    //Loads state information on plugin/project open

    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void RadTriAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    //Saves state information on plugin/project close

    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RadTriAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout RadTriAudioProcessor::createParameters() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterInt>("WIDENER", "Widener", -180.0, 180.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SATURATION", "Saturation", 0.0f, 1.0f, 0.0));

    //Combobox: a gui switch example
    //params.push_back(std::make_unique<juce::AudioParameterChoice>("SHAPE", "Shape", juce::StringArray{ "Sine", "Saw", "Square" }, 0));

    //ADSR
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "A", 0.1f, 1.0f, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "D", 0.1f, 1.0f, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "S", 0.1f, 1.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "R", 0.1f, 3.0f, 0.4f));

    return { params.begin(), params.end() };
}

//Value Tree