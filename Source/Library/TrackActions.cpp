#include "TrackActions.h"


TrackActions::TrackActions(
        TrackInfo &trackInfo,
        const std::function<void(TrackInfo &)> &loadOnDeck1Impl,
        const std::function<void(TrackInfo &)> &loadOnDeck2Impl,
        const std::function<void(TrackInfo &)> &deleteTrackImpl
) : trackInfo(trackInfo),
    loadOnDeck1Impl(loadOnDeck1Impl),
    loadOnDeck2Impl(loadOnDeck2Impl),
    deleteTrackImpl(deleteTrackImpl) {

    addAndMakeVisible(loadOnDeck1Button);
    addAndMakeVisible(loadOnDeck2Button);
    addAndMakeVisible(deleteButton);

    loadOnDeck1Button.addListener(this);
    loadOnDeck2Button.addListener(this);
    deleteButton.addListener(this);
}

TrackActions::~TrackActions() {

}

void TrackActions::paint(Graphics &g) {
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

//    g.setColour(Colours::grey);
//    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
}

void TrackActions::resized() {
    loadOnDeck1Button.setBounds(0, 0, getWidth() / 3, getHeight());
    loadOnDeck2Button.setBounds(getWidth() / 3, 0, getWidth() / 3, getHeight());
    deleteButton.setBounds(getWidth() * 2 / 3, 0, getWidth() / 3, getHeight());
}

void TrackActions::buttonClicked(Button *button) {
    if (button == &loadOnDeck1Button) {
        loadOnDeck1Impl(trackInfo);
    }
    if (button == &loadOnDeck2Button) {
        loadOnDeck2Impl(trackInfo);
    }
    if (button == &deleteButton) {
        deleteTrackImpl(trackInfo);
    }
}