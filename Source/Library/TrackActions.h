#pragma once

#include <JuceHeader.h>
#include "TrackInfo.h"

using std::string;
using std::function;

class TrackActions : public Component,
                     public Button::Listener {

public:
    explicit TrackActions(
            TrackInfo &trackInfo,
            const function<void(TrackInfo &)> &loadOnDeck1Impl,
            const function<void(TrackInfo &)> &loadOnDeck2Impl,
            const function<void(TrackInfo &)> &deleteTrackImpl
    );

    ~TrackActions() override;

    void resized() override;

    void buttonClicked(Button *button) override;

private:
    TrackInfo trackInfo;

    TextButton loadOnDeck1Button{"Load on Deck 1"};
    TextButton loadOnDeck2Button{"Load on Deck 2"};
    TextButton deleteButton{"Delete"};

    const function<void(TrackInfo &)> loadOnDeck1Impl;
    const function<void(TrackInfo &)> loadOnDeck2Impl;
    const function<void(TrackInfo &)> deleteTrackImpl;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackActions)
};


