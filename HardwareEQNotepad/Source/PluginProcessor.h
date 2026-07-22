#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

/**
    Hardware EQ Notepad
    --------------------
    Deze plugin verwerkt GEEN audio (pure pass-through). Het is een digitaal
    "geheugensteuntje" voor de instellingen van een externe hardware-eq
    (4-bands: LO / LO-MID / HI-MID / HI, met schelving-schakelaars op
    LO en HI, en een Range-schakelaar x0.25 / x0.5 / x1).

    Frequenties zijn discrete standen (detent-schakelaar op de hardware),
    gain loopt in stappen van 2 dB van -12 tot +12 dB.

    Alle waarden worden opgeslagen via juce::AudioProcessorValueTreeState,
    wat automatisch meegaat met de DAW-sessie (getStateInformation /
    setStateInformation).
*/
class HardwareEQNotepadAudioProcessor : public juce::AudioProcessor
{
public:
    HardwareEQNotepadAudioProcessor();
    ~HardwareEQNotepadAudioProcessor() override;

    // Parameter-IDs, op één plek gedefinieerd zodat Processor en Editor
    // altijd synchroon lopen.
    static constexpr auto loFreqID     = "loFreq";
    static constexpr auto loGainID     = "loGain";
    static constexpr auto loShelfID    = "loShelf";

    static constexpr auto loMidFreqID  = "loMidFreq";
    static constexpr auto loMidGainID  = "loMidGain";

    static constexpr auto hiMidFreqID  = "hiMidFreq";
    static constexpr auto hiMidGainID  = "hiMidGain";

    static constexpr auto hiFreqID     = "hiFreq";
    static constexpr auto hiGainID     = "hiGain";
    static constexpr auto hiShelfID    = "hiShelf";

    static constexpr auto rangeID      = "range";

    juce::AudioProcessorValueTreeState apvts;

    // --- Standaard AudioProcessor overrides ---
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HardwareEQNotepadAudioProcessor)
};
