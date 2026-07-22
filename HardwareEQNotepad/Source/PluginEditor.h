#pragma once

#include "PluginProcessor.h"
#include <juce_audio_processors/juce_audio_processors.h>

/**
    Eén band-kolom: label + frequentie-knop + gain-knop, met optionele
    shelf-toggle eronder. Elke knop toont zijn exacte waarde in een
    tekstveld, zodat je 'm 1-op-1 kunt aflezen naast je hardware.
*/
struct BandComponent : public juce::Component
{
    BandComponent (const juce::String& bandName, const juce::StringArray& freqChoices, bool hasShelfToggle);

    void resized() override;

    juce::Label nameLabel;

    // Freq is een discrete, gedetenteerde stand op de hardware -> ComboBox,
    // geen vrij draaibare knop.
    juce::ComboBox freqBox;
    juce::Label    freqLabel { {}, "Freq" };

    // Gain is ook gedetenteerd (stappen van 2 dB), maar voelt qua bediening
    // nog aan als een knop -> rotary slider met interval-snapping.
    juce::Slider gainSlider   { juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow };
    juce::Label  gainLabel    { {}, "Gain" };

    std::unique_ptr<juce::ToggleButton> shelfToggle;
};

class HardwareEQNotepadAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit HardwareEQNotepadAudioProcessorEditor (HardwareEQNotepadAudioProcessor&);
    ~HardwareEQNotepadAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    HardwareEQNotepadAudioProcessor& processorRef;

    juce::Label titleLabel;
    juce::Label versionLabel;

    BandComponent loBand    { "LO",     { "30 Hz", "40 Hz", "50 Hz", "100 Hz", "200 Hz", "300 Hz", "400 Hz" },       true };
    BandComponent loMidBand { "LO-MID", { "75 Hz", "150 Hz", "180 Hz", "240 Hz", "500 Hz", "700 Hz", "1 kHz" },      false };
    BandComponent hiMidBand { "HI-MID", { "800 Hz", "1.5 kHz", "3 kHz", "5 kHz", "8 kHz", "10 kHz", "12.5 kHz" },    false };
    BandComponent hiBand    { "HI",     { "2.5 kHz", "5 kHz", "7 kHz", "10 kHz", "12.5 kHz", "15 kHz", "20 kHz" },   true };

    juce::Label rangeLabel { {}, "Range" };
    juce::ComboBox rangeBox;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    std::unique_ptr<ComboBoxAttachment> loFreqAttachment, loMidFreqAttachment, hiMidFreqAttachment, hiFreqAttachment;
    std::unique_ptr<SliderAttachment>   loGainAttachment, loMidGainAttachment, hiMidGainAttachment, hiGainAttachment;

    std::unique_ptr<ButtonAttachment> loShelfAttachment, hiShelfAttachment;
    std::unique_ptr<ComboBoxAttachment> rangeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HardwareEQNotepadAudioProcessorEditor)
};
