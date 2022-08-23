#pragma once

#include <JuceHeader.h>

#include "Headers.h"
#include "Playback/DJAudioPlayer.h"
#include "Playback/TrackDeck.h"
#include "Library/Library.h"


class MainComponent : public AudioAppComponent {
public:
    //==============================================================================
    MainComponent();

    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;

    void releaseResources() override;

    void resized() override;

private:
    //audio utils
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{100};
    MixerAudioSource mixerSource;

    //audio players
    DJAudioPlayer player1{formatManager};
    DJAudioPlayer player2{formatManager};

    //visual components
    Headers headers;
    TrackDeck trackDeck1{&player1, formatManager, thumbCache};
    TrackDeck trackDeck2{&player2, formatManager, thumbCache};
    Library library{
            formatManager,
            [this](const TrackInfo &trackInfo) { trackDeck1.loadFile(trackInfo.filePath); },
            [this](const TrackInfo &trackInfo) { trackDeck2.loadFile(trackInfo.filePath); }
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
