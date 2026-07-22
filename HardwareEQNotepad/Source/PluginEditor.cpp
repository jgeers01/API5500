#include "PluginEditor.h"

#ifndef PLUGIN_VERSION_STRING
 #define PLUGIN_VERSION_STRING "0.0.0-dev"
#endif

// ============================================================================
// BandComponent
// ============================================================================
BandComponent::BandComponent (const juce::String& bandName, const juce::StringArray& freqChoices, bool hasShelfToggle)
{
    nameLabel.setText (bandName, juce::dontSendNotification);
    nameLabel.setJustificationType (juce::Justification::centred);
    nameLabel.setFont (juce::Font (18.0f, juce::Font::bold));
    addAndMakeVisible (nameLabel);

    freqBox.addItemList (freqChoices, 1);
    addAndMakeVisible (freqBox);
    freqLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (freqLabel);

    gainSlider.setRange (-12.0, 12.0, 2.0); // moet gelijk lopen aan de parameter-range in de processor
    gainSlider.setTextValueSuffix (" dB");
    addAndMakeVisible (gainSlider);
    gainLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (gainLabel);

    if (hasShelfToggle)
    {
        shelfToggle = std::make_unique<juce::ToggleButton> ("Shelf");
        addAndMakeVisible (*shelfToggle);
    }
}

void BandComponent::resized()
{
    auto area = getLocalBounds().reduced (4);

    nameLabel.setBounds (area.removeFromTop (24));

    auto freqArea = area.removeFromTop (60);
    freqLabel.setBounds (freqArea.removeFromTop (16));
    freqBox.setBounds (freqArea.reduced (4).withSizeKeepingCentre (freqArea.getWidth() - 8, 26));

    auto gainArea = area.removeFromTop (110);
    gainLabel.setBounds (gainArea.removeFromTop (16));
    gainSlider.setBounds (gainArea);

    if (shelfToggle != nullptr)
    {
        auto toggleArea = area.removeFromTop (28);
        shelfToggle->setBounds (toggleArea.withSizeKeepingCentre (80, 24));
    }
}

// ============================================================================
// HardwareEQNotepadAudioProcessorEditor
// ============================================================================
HardwareEQNotepadAudioProcessorEditor::HardwareEQNotepadAudioProcessorEditor (
    HardwareEQNotepadAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    titleLabel.setText ("Hardware EQ Notepad", juce::dontSendNotification);
    titleLabel.setFont (juce::Font (22.0f, juce::Font::bold));
    titleLabel.setJustificationType (juce::Justification::centredLeft);
    addAndMakeVisible (titleLabel);

    versionLabel.setText (juce::String ("v") + PLUGIN_VERSION_STRING, juce::dontSendNotification);
    versionLabel.setFont (juce::Font (13.0f));
    versionLabel.setJustificationType (juce::Justification::centredRight);
    versionLabel.setColour (juce::Label::textColourId, juce::Colours::grey);
    addAndMakeVisible (versionLabel);

    addAndMakeVisible (loBand);
    addAndMakeVisible (loMidBand);
    addAndMakeVisible (hiMidBand);
    addAndMakeVisible (hiBand);

    rangeLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (rangeLabel);

    rangeBox.addItemList ({ "x0.25", "x0.5", "x1" }, 1);
    addAndMakeVisible (rangeBox);

    auto& apvts = processorRef.apvts;

    loFreqAttachment    = std::make_unique<ComboBoxAttachment> (apvts, HardwareEQNotepadAudioProcessor::loFreqID,    loBand.freqBox);
    loGainAttachment    = std::make_unique<SliderAttachment>   (apvts, HardwareEQNotepadAudioProcessor::loGainID,    loBand.gainSlider);
    loShelfAttachment   = std::make_unique<ButtonAttachment>   (apvts, HardwareEQNotepadAudioProcessor::loShelfID,   *loBand.shelfToggle);

    loMidFreqAttachment = std::make_unique<ComboBoxAttachment> (apvts, HardwareEQNotepadAudioProcessor::loMidFreqID, loMidBand.freqBox);
    loMidGainAttachment = std::make_unique<SliderAttachment>   (apvts, HardwareEQNotepadAudioProcessor::loMidGainID, loMidBand.gainSlider);

    hiMidFreqAttachment = std::make_unique<ComboBoxAttachment> (apvts, HardwareEQNotepadAudioProcessor::hiMidFreqID, hiMidBand.freqBox);
    hiMidGainAttachment = std::make_unique<SliderAttachment>   (apvts, HardwareEQNotepadAudioProcessor::hiMidGainID, hiMidBand.gainSlider);

    hiFreqAttachment    = std::make_unique<ComboBoxAttachment> (apvts, HardwareEQNotepadAudioProcessor::hiFreqID,    hiBand.freqBox);
    hiGainAttachment    = std::make_unique<SliderAttachment>   (apvts, HardwareEQNotepadAudioProcessor::hiGainID,    hiBand.gainSlider);
    hiShelfAttachment   = std::make_unique<ButtonAttachment>   (apvts, HardwareEQNotepadAudioProcessor::hiShelfID,   *hiBand.shelfToggle);

    rangeAttachment     = std::make_unique<ComboBoxAttachment> (apvts, HardwareEQNotepadAudioProcessor::rangeID, rangeBox);

    setSize (640, 340);
    setResizable (false, false);
}

HardwareEQNotepadAudioProcessorEditor::~HardwareEQNotepadAudioProcessorEditor() = default;

void HardwareEQNotepadAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff2b2b2e));

    g.setColour (juce::Colour (0xff3a3a3e));
    g.fillRoundedRectangle (getLocalBounds().reduced (8).toFloat(), 6.0f);
}

void HardwareEQNotepadAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced (16);

    auto header = area.removeFromTop (32);
    titleLabel.setBounds (header.removeFromLeft (header.getWidth() - 80));
    versionLabel.setBounds (header);

    area.removeFromTop (8);

    auto bandsArea = area.removeFromTop (250);
    auto bandWidth = bandsArea.getWidth() / 4;

    loBand.setBounds    (bandsArea.removeFromLeft (bandWidth));
    loMidBand.setBounds (bandsArea.removeFromLeft (bandWidth));
    hiMidBand.setBounds (bandsArea.removeFromLeft (bandWidth));
    hiBand.setBounds    (bandsArea);

    area.removeFromTop (8);

    auto rangeArea = area.removeFromTop (50);
    rangeLabel.setBounds (rangeArea.removeFromLeft (60));
    rangeBox.setBounds (rangeArea.withSizeKeepingCentre (120, 28));
}
