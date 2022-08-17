cmake_minimum_required(VERSION 3.15)

project(OTODECKS VERSION 1.0.0)

add_subdirectory(JUCE)

juce_add_gui_app(Otodecks
    PRODUCT_NAME "Otodecks")

juce_generate_juce_header(Otodecks)

target_sources(Otodecks
    PRIVATE
        Source/TrackDeck.cpp
        Source/DJAudioPlayer.cpp
        Source/WaveformDisplay.cpp
        Source/Library/Library.cpp
        Source/Main.cpp
        Source/Headers.cpp
        Source/Library/TrackInfo.cpp
        Source/Library/TrackStorage.cpp
        Source/Library/TrackActions.cpp
        Source/MainComponent.cpp)

target_compile_definitions(Otodecks
    PRIVATE
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:Otodecks,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:Otodecks,JUCE_VERSION>")

target_link_libraries(Otodecks
    PRIVATE
        juce::juce_gui_extra
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
