#include "PluginProcessor.h"
#include "PluginEditor.h"

HardwareEQNotepadAudioProcessor::HardwareEQNotepadAudioProcessor()
    : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

HardwareEQNotepadAudioProcessor::~HardwareEQNotepadAudioProcessor() = default;

namespace
{
    // Gain: -12 .. +12 dB in stappen van 2 dB (13 standen), default 0 dB.
    juce::NormalisableRange<float> makeGainRange()
    {
        return juce::NormalisableRange<float> (-12.0f, 12.0f, 2.0f);
    }

    juce::AudioParameterFloatAttributes gainAttributes()
    {
        // Toon een expliciete '+' voor positieve waarden en 'dB' als label,
        // zodat de tekst in de plugin exact overeenkomt met de aanduiding
        // op de hardware (bv. "+6 dB" / "-4 dB").
        return juce::AudioParameterFloatAttributes()
            .withLabel ("dB")
            .withStringFromValueFunction ([] (float value, int)
            {
                return (value > 0.0f ? "+" : "") + juce::String (value, 0);
            });
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout
HardwareEQNotepadAudioProcessor::createParameterLayout()
{
    using namespace juce;
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    // --- LO band -----------------------------------------------------
    params.push_back (std::make_unique<AudioParameterChoice> (
        ParameterID { loFreqID, 1 }, "LO Freq",
        StringArray { "30 Hz", "40 Hz", "50 Hz", "100 Hz", "200 Hz", "300 Hz", "400 Hz" },
        0));

    params.push_back (std::make_unique<AudioParameterFloat> (
        ParameterID { loGainID, 1 }, "LO Gain", makeGainRange(), 0.0f, gainAttributes()));

    params.push_back (std::make_unique<AudioParameterBool> (
        ParameterID { loShelfID, 1 }, "LO Shelf", false));

    // --- LO-MID band ---------------------------------------------------
    params.push_back (std::make_unique<AudioParameterChoice> (
        ParameterID { loMidFreqID, 1 }, "LO-MID Freq",
        StringArray { "75 Hz", "150 Hz", "180 Hz", "240 Hz", "500 Hz", "700 Hz", "1 kHz" },
        0));

    params.push_back (std::make_unique<AudioParameterFloat> (
        ParameterID { loMidGainID, 1 }, "LO-MID Gain", makeGainRange(), 0.0f, gainAttributes()));

    // --- HI-MID band ---------------------------------------------------
    params.push_back (std::make_unique<AudioParameterChoice> (
        ParameterID { hiMidFreqID, 1 }, "HI-MID Freq",
        StringArray { "800 Hz", "1.5 kHz", "3 kHz", "5 kHz", "8 kHz", "10 kHz", "12.5 kHz" },
        0));

    params.push_back (std::make_unique<AudioParameterFloat> (
        ParameterID { hiMidGainID, 1 }, "HI-MID Gain", makeGainRange(), 0.0f, gainAttributes()));

    // --- HI band -------------------------------------------------------
    params.push_back (std::make_unique<AudioParameterChoice> (
        ParameterID { hiFreqID, 1 }, "HI Freq",
        StringArray { "2.5 kHz", "5 kHz", "7 kHz", "10 kHz", "12.5 kHz", "15 kHz", "20 kHz" },
        0));

    params.push_back (std::make_unique<AudioParameterFloat> (
        ParameterID { hiGainID, 1 }, "HI Gain", makeGainRange(), 0.0f, gainAttributes()));

    params.push_back (std::make_unique<AudioParameterBool> (
        ParameterID { hiShelfID, 1 }, "HI Shelf", false));

    // --- Range schakelaar (x0.25 / x0.5 / x1) --------------------------
    params.push_back (std::make_unique<AudioParameterChoice> (
        ParameterID { rangeID, 1 }, "Range",
        StringArray { "x0.25", "x0.5", "x1" }, 2)); // default = x1

    return { params.begin(), params.end() };
}

void HardwareEQNotepadAudioProcessor::prepareToPlay (double, int) {}
void HardwareEQNotepadAudioProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HardwareEQNotepadAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // Alles doorlaten, we raken de audio toch niet aan.
    juce::ignoreUnused (layouts);
    return true;
}
#endif

void HardwareEQNotepadAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                                     juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    // Bewust leeg: dit is een pure pass-through, er wordt geen audio bewerkt.
    juce::ignoreUnused (buffer);
}

juce::AudioProcessorEditor* HardwareEQNotepadAudioProcessor::createEditor()
{
    return new HardwareEQNotepadAudioProcessorEditor (*this);
}

bool HardwareEQNotepadAudioProcessor::hasEditor() const { return true; }

const juce::String HardwareEQNotepadAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HardwareEQNotepadAudioProcessor::acceptsMidi() const { return false; }
bool HardwareEQNotepadAudioProcessor::producesMidi() const { return false; }
bool HardwareEQNotepadAudioProcessor::isMidiEffect() const { return false; }
double HardwareEQNotepadAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int HardwareEQNotepadAudioProcessor::getNumPrograms() { return 1; }
int HardwareEQNotepadAudioProcessor::getCurrentProgram() { return 0; }
void HardwareEQNotepadAudioProcessor::setCurrentProgram (int) {}
const juce::String HardwareEQNotepadAudioProcessor::getProgramName (int) { return {}; }
void HardwareEQNotepadAudioProcessor::changeProgramName (int, const juce::String&) {}

void HardwareEQNotepadAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Slaat alle parameters (dus jouw ingevoerde eq-instellingen) op in de DAW-sessie.
    if (auto state = apvts.copyState(); true)
    {
        std::unique_ptr<juce::XmlElement> xml (state.createXml());
        copyXmlToBinary (*xml, destData);
    }
}

void HardwareEQNotepadAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr && xmlState->hasTagName (apvts.state.getType()))
        apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

// Deze functie moet JUCE kunnen vinden om de plugin-instantie te maken.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HardwareEQNotepadAudioProcessor();
}
