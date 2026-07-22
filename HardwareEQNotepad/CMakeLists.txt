cmake_minimum_required(VERSION 3.22)

# ---------------------------------------------------------------------------
# PLUGIN VERSIE
# Verhoog dit nummer bij elke aanpassing die je wilt kunnen terugvinden.
# ---------------------------------------------------------------------------
set(PLUGIN_VERSION "0.2.0")

project(HardwareEQNotepad VERSION ${PLUGIN_VERSION})

# Pad naar JUCE. Clone JUCE ernaast met:
#   git clone --branch 8.0.4 --depth 1 https://github.com/juce-framework/JUCE.git
# zodat je deze structuur krijgt:
#   HardwareEQNotepad/
#   JUCE/
add_subdirectory(../JUCE JUCE_build)

juce_add_plugin(HardwareEQNotepad
    COMPANY_NAME "EigenGebruik"
    PLUGIN_MANUFACTURER_CODE Selg
    PLUGIN_CODE Hweq
    FORMATS VST3 AU
    PRODUCT_NAME "Hardware EQ Notepad"
    VERSION ${PLUGIN_VERSION}
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
)

target_sources(HardwareEQNotepad
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
)

target_compile_definitions(HardwareEQNotepad
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        PLUGIN_VERSION_STRING="${PLUGIN_VERSION}"
)

target_link_libraries(HardwareEQNotepad
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
